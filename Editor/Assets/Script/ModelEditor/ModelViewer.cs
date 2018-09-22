using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Block;

public class ModelViewer
{
    const int SpaceX = 4;
    const int SpaceY = 4;
    const int SpaceZ = 4;
    static readonly Vector3 spaceSize = new Vector3(SpaceX, SpaceY, SpaceZ) * Const.ChunkSize;
    static readonly Vector3 halfSpaceSize = spaceSize * 0.5f;
    Camera camera;
    List<GameObject> objects = new List<GameObject>();
    private Vector3[] gridLines;
    struct MeshInfo
    {
        public MeshInfo(Mesh m, Vector3 p) {
            mesh = m;
            pos = p;
        }
        public Mesh mesh;
        public Vector3 pos;
    }
    //List<MeshInfo> m_meshes = new List<MeshInfo>();
    Mesh previewMesh = null;

    public BlockManager blockManager;
    private EditorCommandManager commandManager;
    RayCastManager rtm;
    bool bDirty = false;
    private bool bShowPreview = false;
    private bool bNeedReselectPreview = false;
    private Vector3 selectedBlockPosStart;
    private Vector3 selectedBlockPosEnd2D;
    private Vector3 selectedBlockPosEnd3D;
    private Vector3 previewBlockPosStart;
    private Vector3 previewBlockPosEnd2D;
    private Vector3 previewBlockPosEnd3D;
    private Vector2 mouseDownPos;
    private Vector2 lastMousePos;
    private Face normalIndex = 0;
    public enum Operator
    {
        DoDothing = -1,
        AddBlock,
        DeleteBlock,
    }
    public Operator curOperator = Operator.AddBlock;
    public Game.BlockType curBlockType = Game.BlockType.Sand;
    public enum SelectBlockStage
    {
        //DoNothing,
        //Preview,
        SelectStart,
        SelectEnd2D,
        SelectEnd3D,
    }
    private SelectBlockStage selectBlockStage;
    Vector3[][] rays;
    public void Init(Camera cam,EditorCommandManager commandManager) {
        //计算格子
        List<Vector3> tempLines = new List<Vector3>();
        for (int i = 0; i <= SpaceX * Const.ChunkSize; i++) {
            tempLines.Add(new Vector3(i, halfSpaceSize.y, 0) - halfSpaceSize);
            tempLines.Add(new Vector3(i, halfSpaceSize.y, SpaceY * Const.ChunkSize) - halfSpaceSize);
        }
        for (int j = 0; j <= SpaceY * Const.ChunkSize; j++) {
            tempLines.Add(new Vector3(0, halfSpaceSize.y, j) - halfSpaceSize);
            tempLines.Add(new Vector3(SpaceX * Const.ChunkSize, halfSpaceSize.y, j) - halfSpaceSize);
        }
        gridLines = tempLines.ToArray();
            //预处理光线
            rays = new Vector3[6][];
        {
            for (int i = 0; i < 6; i++) {
                rays[i] = TerrainTool.getRandomRays(9, i);
            }
        }
        DebugTool.Log("预处理光线追踪");

        camera = cam;
        this.commandManager = commandManager;
        blockManager = new BlockManager();
        blockManager.create(SpaceX * Const.ChunkSize, SpaceY * Const.ChunkSize, SpaceZ * Const.ChunkSize, new BlockTypeFun());
        //blockManager.forEachChunk((BlockChunk chunk, int i, int j, int k) => {
        //    for (int x = 0; x < Const.ChunkSize; x++) {
        //        for (int y = 0; y < Const.ChunkSize; y++) {
        //            for (int z = 0; z < Const.ChunkSize; z++) {
        //                int gx = i * Const.ChunkSize + x;
        //                int gy = j * Const.ChunkSize + y;
        //                int gz = k * Const.ChunkSize + z;
        //                if ((new Vector3(gx, gy, gz) - new Vector3(SpaceX / 2.0f, SpaceY / 2.0f, SpaceZ / 2.0f) * Const.ChunkSize).magnitude < 10) {
        //                    chunk.setBlock(x, y, z, (int)BlockTypeEnum.Sand);
        //                }
        //            }
        //        }
        //    }
        //});

        rtm = new RayCastManager();
        rtm.create(blockManager.SizeX, blockManager.SizeY, blockManager.SizeZ);
        //rtm.moveTo(0, 0, 0, (int gx, int gy, int gz) => {
        //    return blockManager.getBlock(gx, gy, gz) != 0 ? RayCastBlockType.All : RayCastBlockType.Nothing;
        //});
        for (int x = 0; x < SpaceX * Const.ChunkSize; x++) {
            for (int y = 0; y < SpaceY * Const.ChunkSize; y++) {
                for (int z = 0; z < SpaceZ * Const.ChunkSize; z++) {
                    rtm.setBlock(x, y, z, blockManager.getBlock(x, y, z) != 0 ? RayCastBlockType.All : RayCastBlockType.Nothing);
                }
            }
        }

        bDirty = true;

        previewMesh = GLRender.createCubeMesh(0.5f,Color.white);
    }

    public void SetDirty() {
        bDirty = true;
    }
    public void SetOperator(Operator op) {
        curOperator = op;
        if (curOperator == Operator.DeleteBlock) {
            UpdateSelect(lastMousePos);
        }
    }
    public void Clear() {
        blockManager.clear();
        bDirty = true;
    }
    public VecInt3 GetPivot()
    {
        return new VecInt3(SpaceX * Const.ChunkSize/2,SpaceY * Const.ChunkSize/2,SpaceZ * Const.ChunkSize/2);
    }
    public void Update() {
        if (bDirty) {
            bDirty = false;
            bNeedReselectPreview = true;
            DebugTool.Log("开始更新射线模块");
            rtm.clearAll();
            blockManager.forEachBlock((short block, int gx, int gy, int gz) => {
                if (block != (short)BlockTypeEnum.Air) {
                    rtm.setBlock(gx, gy, gz, RayCastBlockType.All);
                }
            });

            //初始化AO计算
            RayMarchingAo rma = new RayMarchingAo();
            rma.Init(rtm);

            foreach (var obj in objects) {
                GameObject.Destroy(obj);
            }
            objects.Clear();

            //m_meshes.Clear();
            DebugTool.Log("开始计算光照"); 
            {
                //计算光照
                Vector3 sunDir = new Vector3(1, -3, 1);
                sunDir.Normalize();
                TerrainTool.calcLight(blockManager, rtm, rays, sunDir);
            }

            BlockTypeFun blockTypeFun = new BlockTypeFun();

            //TerrainTool.calcLight2(blockManager, 16);
            DebugTool.Log("开始更新网格");
            for (int i = 0; i < SpaceX; i++) {
                for (int k = 0; k < SpaceZ; k++) {
                    for (int j = 0; j < SpaceY; j++) {
                        BlockChunk chunk = new BlockChunk(blockManager, i * Const.ChunkSize, j * Const.ChunkSize, k * Const.ChunkSize);
                        for (int f = 0; f < 6; f++) {
                            List<MeshTool.BlockSurface> surface = MeshTool.getChunkSurface(chunk, blockTypeFun, f);
                            Texture2D texSurface = MeshTool.SurfacePointsToTexture(surface, f);

                            Texture2D readback = null;
                            if (surface.Count > 0) {
                                RenderTexture targetAoResult = rma.RenderByCalcShader(texSurface, new Vector3(i, j, k) * Const.ChunkSize, f);
                                //回读亮度数据
                                RenderTexture.active = targetAoResult;
                                readback = new Texture2D(targetAoResult.width, targetAoResult.height);
                                readback.ReadPixels(new Rect(0, 0, targetAoResult.width, targetAoResult.width), 0, 0);
                                MeshTool.SetRaytraceAo(surface, readback);
                            }

                            Mesh mesh = MeshTool.createMesh2(surface, f, blockTypeFun);
                            if (mesh) {
                                Vector3 pos = ((new Vector3(i, j, k))) * Const.ChunkSize - halfSpaceSize;
                                if (mesh != null) {
                                    GameObject obj = new GameObject("Chunk", typeof(MeshRenderer), typeof(MeshFilter));
                                    obj.isStatic = true;
                                    obj.GetComponent<Renderer>().material = GlobalResources.getBlockMaterial();
                                    obj.GetComponent<MeshFilter>().mesh = mesh;
                                    obj.transform.position = pos;
                                    objects.Add(obj);
                                }
                            }
                        }
                    }
                }
            }
            DebugTool.Log("结束");
        }
    }

    public void OnGUI() {
        //Event e = Event.current;

        Vector2 touchPos = new Vector2(Input.mousePosition.x,camera.pixelHeight - Input.mousePosition.y);
        bool isKeyDown_Ctrl = Input.GetKeyDown(KeyCode.LeftControl);
        bool isMouseDown = Input.GetMouseButtonDown(0);
        bool isMouseLeftUp = Input.GetMouseButtonUp(0);
        bool isMouseLeftDown = Input.GetMouseButtonDown(0);

        if (Input.touchCount >= 2) {
            isMouseDown = false;
            selectBlockStage = SelectBlockStage.SelectStart;
        }

        if (camera.pixelRect.Contains(touchPos)) {
            if (isKeyDown_Ctrl) {
                if (selectBlockStage == SelectBlockStage.SelectEnd2D) {
                    selectBlockStage = SelectBlockStage.SelectEnd3D;
                }
            }
            if (isMouseDown && selectBlockStage == SelectBlockStage.SelectStart) {
                if (isMouseLeftDown && bShowPreview) {
                    mouseDownPos = touchPos;
                    lastMousePos = mouseDownPos;
                    UpdateSelect(touchPos);
                    selectBlockStage = SelectBlockStage.SelectEnd2D;
                    UpdateSelect(touchPos);
                }
            }
            else if (isMouseLeftUp) {
                if (selectBlockStage == SelectBlockStage.SelectEnd3D || selectBlockStage == SelectBlockStage.SelectEnd2D) {
                    selectBlockStage = SelectBlockStage.SelectStart;
                    if (curOperator == Operator.AddBlock) {
                        if (bShowPreview) {
                            Vector3 diff = previewBlockPosEnd3D - previewBlockPosStart;
                            if ((mouseDownPos - touchPos).magnitude < 0.0001) {
                                diff = Vector3.zero;
                            }
                            int nx = (int)(Mathf.Abs(diff.x + 0.01f)) + 1;
                            int ny = (int)(Mathf.Abs(diff.y + 0.01f)) + 1;
                            int nz = (int)(Mathf.Abs(diff.z + 0.01f)) + 1;
                            nx = nx < 100 ? nx : 100;
                            ny = ny < 100 ? ny : 100;
                            nz = nz < 100 ? nz : 100;
                            int stepX = diff.x > 0 ? 1 : -1;
                            int stepY = diff.y > 0 ? 1 : -1;
                            int stepZ = diff.z > 0 ? 1 : -1;
                            int gx = (int)previewBlockPosStart.x;
                            int gy = (int)previewBlockPosStart.y;
                            int gz = (int)previewBlockPosStart.z;
                            EditorCommandSet cmd = new EditorCommandSet(gx, gy, gz, nx * stepX, ny * stepY, nz * stepZ, blockManager, (short)curBlockType);
                            commandManager.Do(cmd, blockManager);
                            bDirty = true;
                        }
                    }
                    if (curOperator == Operator.DeleteBlock) {
                        if (bShowPreview) {
                            Vector3 diff = selectedBlockPosEnd3D - selectedBlockPosStart;
                            if ((mouseDownPos - touchPos).magnitude < 0.0001) {
                                diff = Vector3.zero;
                            }
                            int nx = (int)(Mathf.Abs(diff.x + 0.01f)) + 1;
                            int ny = (int)(Mathf.Abs(diff.y + 0.01f)) + 1;
                            int nz = (int)(Mathf.Abs(diff.z + 0.01f)) + 1;
                            nx = nx < 100 ? nx : 100;
                            ny = ny < 100 ? ny : 100;
                            nz = nz < 100 ? nz : 100;
                            int stepX = diff.x >= 0 ? 1 : -1;
                            int stepY = diff.y >= 0 ? 1 : -1;
                            int stepZ = diff.z >= 0 ? 1 : -1;
                            int gx = (int)selectedBlockPosStart.x;
                            int gy = (int)selectedBlockPosStart.y;
                            int gz = (int)selectedBlockPosStart.z;
                            EditorCommandSet cmd = new EditorCommandSet(gx, gy, gz, nx * stepX, ny * stepY, nz * stepZ, blockManager, (short)BlockTypeEnum.Air);
                            commandManager.Do(cmd, blockManager);
                            bDirty = true;
                        }
                    }
                }
            }
            if (touchPos != lastMousePos || bNeedReselectPreview) {
                bNeedReselectPreview = false;
                if (curOperator == Operator.AddBlock || curOperator == Operator.DeleteBlock) {
                    UpdateSelect(touchPos);
                }
                lastMousePos = touchPos;
            }
        }

    }

    private void UpdateSelect(Vector2 mousePosition)
    {
        Ray ray = camera.ScreenPointToRay(new Vector3(mousePosition.x, camera.pixelHeight - mousePosition.y, 0));
        Vector3 startPos = ray.origin + halfSpaceSize;
        Vector3 dir = ray.direction;
        if (selectBlockStage == SelectBlockStage.SelectStart) {
            RayCastRestult rlt = rtm.rayCast(startPos, dir, 200,RayCastBlockType.All);
            bShowPreview = rlt.bHit;
            if (rlt.bHit) {
                normalIndex = (Face)rlt.hitFaceIndex;
                Vector3 hitPos = startPos + dir * (rlt.hitLength + 0.001f);
                float gx = (int)(hitPos.x / Const.BlockSize);
                float gy = (int)(hitPos.y / Const.BlockSize);
                float gz = (int)(hitPos.z / Const.BlockSize);
                if (curOperator == Operator.AddBlock) {
                    selectedBlockPosStart = new Vector3(gx, gy, gz);
                    selectedBlockPosEnd2D = selectedBlockPosStart;
                    previewBlockPosStart = new Vector3(gx, gy, gz) + Const.getFaceNormal(rlt.hitFaceIndex) * Const.BlockSize;
                    previewBlockPosEnd2D = previewBlockPosStart;
                    previewBlockPosEnd3D = previewBlockPosEnd2D;
                }
                else if (curOperator == Operator.DeleteBlock) {
                    selectedBlockPosStart = new Vector3(gx, gy, gz);
                    selectedBlockPosEnd2D = selectedBlockPosStart;
                    selectedBlockPosEnd3D = selectedBlockPosEnd2D;
                }
            }
            else {
                normalIndex = Face.FNI_y1;
                Vector3 faceNormal = Vector3.up;//表面法线
                float camToFace = startPos.y - halfSpaceSize.y;//摄像机到表面的垂直线
                float factor = Vector3.Dot(-faceNormal, dir);
                if (factor > 0.0001) {
                    Vector3 hitPos = startPos + dir * (camToFace / factor + 0.001f);
                    float gx = (int)(hitPos.x / Const.BlockSize);
                    float gy = (int)(hitPos.y / Const.BlockSize);
                    float gz = (int)(hitPos.z / Const.BlockSize);
                    if (curOperator == Operator.AddBlock) {
                        previewBlockPosStart = new Vector3(gx, gy, gz) + Vector3.up * Const.BlockSize;
                        previewBlockPosEnd2D = previewBlockPosStart;
                        previewBlockPosEnd3D = previewBlockPosEnd2D;
                    }
                    else if (curOperator == Operator.DeleteBlock) {
                        selectedBlockPosStart = new Vector3(gx, gy, gz);
                        selectedBlockPosEnd2D = selectedBlockPosStart;
                        selectedBlockPosEnd3D = selectedBlockPosEnd2D;
                    }
                    bShowPreview = true;
                }
                else {
                    bShowPreview = false;
                }
            }
        }
        else if (selectBlockStage == SelectBlockStage.SelectEnd2D) {
            if(curOperator == Operator.AddBlock){
                //计算摄像机在操作平面上的投影点
                Vector3 camToStart = previewBlockPosStart - startPos;//摄像机到开始点
                Vector3 faceNormal = Const.getFaceNormal((int)normalIndex);//表面法线
                float camToFace = Vector3.Dot(camToStart, -faceNormal) + 0.49999f;//摄像机到表面的垂直线
                float factor = Vector3.Dot(-faceNormal, dir);
                if (factor > 0.0001) {
                    previewBlockPosEnd2D = startPos + dir * (camToFace / factor);
                    previewBlockPosEnd3D = previewBlockPosEnd2D;
                    //Debug.Log(previewBlockPosEnd.ToString());
                    bShowPreview = true;
                }
                else {
                    bShowPreview = false;
                }
            }
            else if (curOperator == Operator.DeleteBlock) {
                //计算摄像机在操作平面上的投影点
                Vector3 camToStart = selectedBlockPosStart - startPos;//摄像机到开始点
                Vector3 faceNormal = Const.getFaceNormal((int)normalIndex);//表面法线
                float camToFace = Vector3.Dot(camToStart, -faceNormal) + 0.49999f;
                float factor = Vector3.Dot(-faceNormal, dir);
                if (factor > 0.0001) {
                    selectedBlockPosEnd2D = startPos + dir * (camToFace / factor);
                    selectedBlockPosEnd3D = selectedBlockPosEnd2D;
                    bShowPreview = true;
                }
                else {
                    bShowPreview = false;
                }
            }
        }
        else if (selectBlockStage == SelectBlockStage.SelectEnd3D) {
            if (curOperator == Operator.AddBlock) {
                //计算摄像机在操作平面上的投影点
                Vector3 camToStart = previewBlockPosEnd2D - startPos;//摄像机到开始点
                Vector3 faceNormal = Const.getFaceNormal((int)normalIndex);//击中法线
                Vector3 right = Vector3.Cross(faceNormal, dir).normalized;
                Vector3 forward = Vector3.Cross(right, faceNormal);//操作平面的法线
                float factor = Vector3.Dot(dir, forward);
                if (factor > 0.0001) {
                    bShowPreview = true;
                    Vector3 hitPos = startPos + dir * Vector3.Dot(camToStart, forward) / factor;
                    Vector3 offset = faceNormal * Vector3.Dot((hitPos - previewBlockPosEnd2D), faceNormal);
                    previewBlockPosEnd3D = previewBlockPosEnd2D + offset;
                }
                else {
                    bShowPreview = false;
                }
            }
            else if (curOperator == Operator.DeleteBlock) {
                //计算摄像机在操作平面上的投影点
                Vector3 camToStart = selectedBlockPosEnd2D - startPos;//摄像机到开始点
                Vector3 faceNormal = Const.getFaceNormal((int)normalIndex);//击中法线
                Vector3 right = Vector3.Cross(faceNormal, dir).normalized;
                Vector3 forward = Vector3.Cross(right, faceNormal);//操作平面的法线
                float factor = Vector3.Dot(dir, forward);
                if (factor > 0.0001) {
                    bShowPreview = true;
                    Vector3 hitPos = startPos + dir * Vector3.Dot(camToStart, forward) / factor;
                    Vector3 offset = faceNormal * Vector3.Dot((hitPos - selectedBlockPosEnd2D), faceNormal);
                    selectedBlockPosEnd3D = selectedBlockPosEnd2D + offset;
                }
                else {
                    bShowPreview = false;
                }
            }
        }
    }

    public void OnRenderObject() {
        //GLRender.DrawWireCube(-halfSpaceSize, halfSpaceSize, new Color(1,1,1,0.1f), GlobalResources.getAlphaBlend());
        //foreach (MeshInfo mi in m_meshes) {
        //    Graphics.DrawMesh(mi.mesh, mi.pos - halfSpaceSize, Quaternion.identity, GlobalResources.getBlockMaterial(),0);
        //}
        //if (bShowPreview && curOperator == Operator.DeleteBlock) {
        //    Vector3 min = selectedBlockPosStart - Vector3.one * 0.51f - halfSpaceSize;
        //    Vector3 max = selectedBlockPosStart + Vector3.one * 0.51f - halfSpaceSize;
        //    GLRender.DrawWireCube(min,max ,new Color(1,0,0,0.3f),GlobalResources.getAlphaBlend());
        //}
        GLRender.DrawLinePairs(gridLines, new Color(1,1,1,0.1f), GlobalResources.getAlphaBlend());
        GLRender.DrawLine(Vector3.Scale(new Vector3(0,0,-0.5f), spaceSize), Vector3.Scale(new Vector3(0,0,0.5f), spaceSize), new Color(1, 1, 1, 0.2f), GlobalResources.getAlphaBlend());
        GLRender.DrawLine(Vector3.Scale(new Vector3(-0.5f,0,0), spaceSize), Vector3.Scale(new Vector3(0.5f,0,0), spaceSize), new Color(1, 1, 1, 0.2f), GlobalResources.getAlphaBlend());
        if (bShowPreview && curOperator == Operator.AddBlock) {
            Vector3 pos = previewBlockPosStart - halfSpaceSize - Vector3.one * 0.51f;
            Vector3 diff = previewBlockPosEnd3D - previewBlockPosStart;
            int nx = (int)(Mathf.Abs(diff.x + 0.01f)) + 1;
            int ny = (int)(Mathf.Abs(diff.y + 0.01f)) + 1;
            int nz = (int)(Mathf.Abs(diff.z + 0.01f)) + 1;
            nx = nx < 100 ? nx : 100;
            ny = ny < 100 ? ny : 100;
            nz = nz < 100 ? nz : 100;
            int stepX = diff.x > 0 ? 1 : -1;
            int stepY = diff.y > 0 ? 1 : -1;
            int stepZ = diff.z > 0 ? 1 : -1;
            GlobalResources.getBlue().SetPass(0);
            for (int x = 0; x < nx; x++) {
                for (int y = 0; y < ny; y++) {
                    for (int z = 0; z < nz; z++) {
                        Matrix4x4 mat = Matrix4x4.TRS(pos + new Vector3(x * stepX, y * stepY, z * stepZ), Quaternion.identity, Vector3.one * 1.02f);
                        Graphics.DrawMeshNow(previewMesh,mat,0);
                        //GLRender.DrawCube(min, max, new Color(1, 0, 0, 0.2f), GlobalResources.getAlphaBlend());
                    }
                }
            }
            Vector3 min = selectedBlockPosStart - halfSpaceSize;
            Vector3 max = selectedBlockPosStart + Vector3.one - halfSpaceSize;
            GLRender.DrawWireCube(min, max, new Color(1, 0, 0, 0.2f), GlobalResources.getAlphaBlend());
        }
        else if (bShowPreview && curOperator == Operator.DeleteBlock) {
            //Vector3 pos = selectedBlockPosStart - halfSpaceSize;
            Vector3 diff = selectedBlockPosEnd3D - selectedBlockPosStart;
            int nx = (int)(Mathf.Abs(diff.x + 0.01f)) + 1;
            int ny = (int)(Mathf.Abs(diff.y + 0.01f)) + 1;
            int nz = (int)(Mathf.Abs(diff.z + 0.01f)) + 1;
            nx = nx < 100 ? nx : 100;
            ny = ny < 100 ? ny : 100;
            nz = nz < 100 ? nz : 100;
            int stepX = diff.x > 0 ? 1 : -1;
            int stepY = diff.y > 0 ? 1 : -1;
            int stepZ = diff.z > 0 ? 1 : -1;
            for (int x = 0; x < nx; x++) {
                for (int y = 0; y < ny; y++) {
                    for (int z = 0; z < nz; z++) {
                        Vector3 center = selectedBlockPosStart - halfSpaceSize + new Vector3(x * stepX, y * stepY, z * stepZ);
                        Vector3 min = center;
                        Vector3 max = center + Vector3.one;
                        GLRender.DrawWireCube(min, max, new Color(1, 0, 0, 0.3f), GlobalResources.getAlphaBlend());
                    }
                }
            }
        }
        //if (bShowPreview && curOperator == Operator.AddBlock) {
        //    Gizmos.matrix = transform.localToWorldMatrix;
        //    Gizmos.color = Color.white;
        //    Gizmos.DrawWireCube(selectedBlockPos, Vector3.one * 1.05f);
        //    Gizmos.DrawCube(previewBlockPos, Vector3.one);
        //}
        //else if (bShowPreview && curOperator == Operator.DeleteBlock) {
        //    Gizmos.matrix = transform.localToWorldMatrix;
        //    Gizmos.color = Color.white;
        //    Gizmos.DrawWireCube(selectedBlockPos, Vector3.one * 1.05f);
        //}
    }
}
