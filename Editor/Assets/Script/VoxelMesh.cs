using UnityEngine;
using System.Collections;
using Block;
using System.Collections.Generic;
using System;

//[ExecuteInEditMode]
public class VoxelMesh
{
    const int SpaceX = 5;
    const int SpaceY = 5;
    const int SpaceZ = 5;
    bool bDirty = false;
    private int CenterX = SpaceX * Const.ChunkSize / 2;
    private int CenterY = 0;//SpaceY * Const.ChunkSize / 2;
    private int CenterZ = SpaceZ * Const.ChunkSize / 2;
    //private Vector3 center;
    private GameObject objSubMesh;

    //private bool bShowPreview = false;
    private Vector3 selectedBlockPos;
    private Vector3 previewBlockPos;
    private Vector2 mouseDownPos;

    public enum Operator
    {
        DoDothing,
        AddBlock,
        DeleteBlock,
    }

    public Operator curOperator;

    public VoxelModel model;

    BlockManager blockManager;
    RayCastManager rtm;

    struct MeshInfo
    {
        public MeshInfo(Mesh m, Vector3 p) {
            mesh = m;
            pos = p;
        }
        public Mesh mesh;
        public Vector3 pos;
    }
    List<MeshInfo> m_meshes = new List<MeshInfo>();
    
    void Start() {
    }

    public void OnDisable()
    {
        Debug.Log("VoxelMesh.OnDisable");
        int minX, minY, minZ;
        int maxX, maxY, maxZ;
        minX = minY = minZ = 99999;
        maxX = maxY = maxZ = -99999;
        blockManager.forEachBlock((short block, int x, int y, int z) => {
            if (block != (short)BlockTypeEnum.Air) {
                minX = Math.Min(x, minX); minY = Math.Min(y, minY); minZ = Math.Min(z, minZ);
                maxX = Math.Max(x, maxX); maxY = Math.Max(y, maxY); maxZ = Math.Max(z, maxZ);
            }
        });
        int dx = maxX - minX + 1;
        int dy = maxY - minY + 1;
        int dz = maxZ - minZ + 1;
        if (dx > 0 && dy > 0 && dz > 0) {
            model.sizeX = dx; model.sizeY = dy; model.sizeZ = dz;
            model.blocks = new short[dx * dy * dz];
            model.originX = minX; model.originY = minY; model.originZ = minZ;
            blockManager.forEachBlock((short block, int x, int y, int z) => {
                if (x >= minX && x<=maxX && y >=minY && y<=maxY && z>=minZ && z<=maxZ) {
                    x -= minX; y -= minY; z -= minZ;
                    model.blocks[z * dx * dy + y * dx + x] = block;
                }
            });
        }
        else {
            model.sizeX = 0; model.sizeY = 0; model.sizeZ = 0;
            model.blocks = new short[0];
            model.originX = CenterX; model.originY = CenterY; model.originZ = CenterZ;
        }
    }

    public void Init() {
        Debug.Log("VoxelMesh.Init");
        //center = new Vector3(CenterX, CenterY, CenterZ);
        blockManager = new BlockManager();
        blockManager.create(SpaceX * Const.ChunkSize, SpaceY * Const.ChunkSize, SpaceZ * Const.ChunkSize, new BlockTypeFun());
        if (model == null) {
            model = new VoxelModel();
        }
        else {
            for (int i = 0; i < model.sizeX; i++) {
                for (int j = 0; j < model.sizeY; j++) {
                    for (int k = 0; k < model.sizeZ; k++) {
                        short b = model.blocks[k*model.sizeX*model.sizeY + j * model.sizeX + i];
                        if (b != (short)BlockTypeEnum.Air) {
                            blockManager.setBlock(i + model.originX, j + model.originY, k + model.originZ, b);
                        }
                    }
                }
            }
        }

        rtm = new RayCastManager();
        rtm.create(blockManager.SizeX, blockManager.SizeY, blockManager.SizeZ);
        for (int x = 0; x < SpaceX * Const.ChunkSize; x++) {
            for (int y = 0; y < SpaceY * Const.ChunkSize; y++) {
                for (int z = 0; z < SpaceZ * Const.ChunkSize; z++) {
                    rtm.setBlock(x, y, z, blockManager.getBlock(x, y, z) != 0 ? RayCastBlockType.All : RayCastBlockType.Nothing);
                }
            }
        }
        bDirty = true;
        curOperator = Operator.DeleteBlock;
    }
    public void clear()
    {
        blockManager.clear();
        bDirty = true;
    }
    // Update is called once per frame
    public void EditorUpdate(Camera cam) {
        //Event e = Event.current;
        //if (e.type == EventType.MouseDown) {
        //    mouseDownPos = e.mousePosition;
        //    if (e.button == 0) {
        //        if (curOperator == Operator.AddBlock) {
        //            if (bShowPreview) {
        //                int gx = (int)previewBlockPos.x;
        //                int gy = (int)previewBlockPos.y;
        //                int gz = (int)previewBlockPos.z;
        //                blockManager.setBlock(gx, gy, gz, (short)BlockTypeEnum.BT_Sand);
        //                bDirty = true;
        //            }
        //        }
        //        if (curOperator == Operator.DeleteBlock) {
        //            if (bShowPreview) {
        //                int gx = (int)selectedBlockPos.x;
        //                int gy = (int)selectedBlockPos.y;
        //                int gz = (int)selectedBlockPos.z;
        //                blockManager.setBlock(gx, gy, gz, (short)BlockTypeEnum.BT_Air);
        //                bDirty = true;
        //            }
        //        }
        //    }
        //}
        //else if (e.type == EventType.mouseMove) {
        //    if (curOperator == Operator.AddBlock || curOperator == Operator.DeleteBlock) {
        //        Ray ray = cam.ScreenPointToRay(new Vector3(e.mousePosition.x, cam.pixelHeight - e.mousePosition.y, 0));
        //        Vector3 startPos = transform.InverseTransformPoint(ray.origin);
        //        Vector3 dir = transform.InverseTransformDirection(ray.direction);
        //        RayCastRestult rlt = rtm.rayCast(startPos, dir, 50);
        //        bShowPreview = rlt.bHit;
        //        if (rlt.bHit) {
        //            //Debug.Log(rlt.hitFaceIndex.ToString());
        //            if (curOperator == Operator.AddBlock) {
        //                Vector3 hitPos = startPos + dir * (rlt.hitLength + 0.001f);
        //                float gx = (int)(hitPos.x / Const.BlockSize) + 0.5f;
        //                float gy = (int)(hitPos.y / Const.BlockSize) + 0.5f;
        //                float gz = (int)(hitPos.z / Const.BlockSize) + 0.5f;
        //                selectedBlockPos = new Vector3(gx, gy, gz);
        //                previewBlockPos = new Vector3(gx, gy, gz) + Const.getFaceNormal(rlt.hitFaceIndex) * Const.BlockSize;
        //            }
        //            else if (curOperator == Operator.DeleteBlock) {
        //                Vector3 hitPos = startPos + dir * (rlt.hitLength + 0.001f);
        //                float gx = (int)(hitPos.x / Const.BlockSize) + 0.5f;
        //                float gy = (int)(hitPos.y / Const.BlockSize) + 0.5f;
        //                float gz = (int)(hitPos.z / Const.BlockSize) + 0.5f;
        //                selectedBlockPos = new Vector3(gx, gy, gz);
        //            }
        //        }
        //    }
        //}

        if(bDirty){
            bDirty = false;
            DebugTool.Log("开始更新光线追踪");
            rtm.clearAll(); 
            blockManager.forEachBlock((short block,int gx, int gy, int gz) => {
                if (block != (short)BlockTypeEnum.Air) {
                    rtm.setBlock(gx, gy, gz, RayCastBlockType.All);
                }
            });

            //初始化AO计算
            RayMarchingAo rma = new RayMarchingAo();
            rma.Init(rtm);

            m_meshes.Clear();
            DebugTool.Log("开始计算光照");
            TerrainTool.calcLight2(blockManager, 32);
            DebugTool.Log("开始更新网格");

            BlockTypeFun blockTypeFun = new BlockTypeFun();

            for (int i = 0; i < SpaceX; i++) {
                for (int k = 0; k < SpaceZ; k++) {
                    for (int j = 0; j < SpaceY; j++) {
                        BlockChunk chunk = new BlockChunk(blockManager, i * Const.ChunkSize, j * Const.ChunkSize, k * Const.ChunkSize);
                        for (int f = 0; f < 6; f++) {
                            List<MeshTool.BlockSurface> surface = MeshTool.getChunkSurface(chunk, blockTypeFun, f);
                            Texture2D texSurface = MeshTool.SurfacePointsToTexture(surface, f);

                            RenderTexture targetAoResult = rma.RenderByCalcShader(texSurface, new Vector3(i, j, k) * Const.ChunkSize, f);
                            //回读亮度数据
                            RenderTexture.active = targetAoResult;
                            Texture2D readback = new Texture2D(targetAoResult.width, targetAoResult.height);
                            readback.ReadPixels(new Rect(0, 0, targetAoResult.width, targetAoResult.width), 0, 0);
                            MeshTool.SetRaytraceAo(surface, readback);
                            Mesh mesh = MeshTool.createMesh2(surface, f, blockTypeFun);
                            if (mesh) {
                                Vector3 pos = ((new Vector3(i, j, k))) * Const.ChunkSize;
                                m_meshes.Add(new MeshInfo(mesh, pos));
                            }
                        }
                    }
                }
            }
            DebugTool.Log("结束");
        }
    }

    public void OnDrawGizmosSelected()
    {
        //Gizmos.matrix = transform.localToWorldMatrix;
        //Gizmos.color = Color.white;
        //Vector3 spaceSize = new Vector3(SpaceX, SpaceY, SpaceZ) * Const.ChunkSize;
        //Gizmos.DrawWireCube(Vector3.zero + spaceSize * 0.5f, spaceSize);

        //Gizmos.color = Color.red;
        //Gizmos.DrawRay(center, new Vector3(1, 0, 0) * (SpaceX * Const.ChunkSize - CenterX));
        //Gizmos.color = Color.green;
        //Gizmos.DrawRay(center, new Vector3(0, 1, 0) * (SpaceY * Const.ChunkSize - CenterY));
        //Gizmos.color = Color.blue;
        //Gizmos.DrawRay(center, new Vector3(0, 0, 1) * (SpaceZ * Const.ChunkSize - CenterZ));
    }

    public void OnRenderObject() {
        //TerrainTool.debugTool.drawAllLines();
        //GlobalResources.getBlockMaterial().SetPass(0);
        //Gizmos.color = new Color(0, 0, 0, 0);
        //foreach (MeshInfo mi in m_meshes) {
        //    Matrix4x4 matrixLocal = Matrix4x4.TRS(mi.pos, Quaternion.identity, Vector3.one);
        //    Matrix4x4 finalMatrix = transform.localToWorldMatrix * matrixLocal;
        //    Gizmos.matrix = finalMatrix;
        //    Graphics.DrawMeshNow(mi.mesh, finalMatrix);
        //    Gizmos.DrawMesh(mi.mesh);
        //}
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

   
    void OnDrawGizmos() {
        //Gizmos.DrawRay()

        //TerrainTool.debugTool.drawAllLines();
        //GlobalResources.getBlockMaterial().SetPass(0);
        //Gizmos.color = new Color(0, 0, 0, 0);
        //foreach (MeshInfo mi in m_meshes) {
        //    Matrix4x4 matrixLocal = Matrix4x4.TRS(mi.pos, Quaternion.identity, Vector3.one);
        //    Matrix4x4 finalMatrix = transform.localToWorldMatrix * matrixLocal;
        //    Gizmos.matrix = finalMatrix;
        //    Graphics.DrawMeshNow(mi.mesh, finalMatrix);
        //    Gizmos.DrawMesh(mi.mesh);
        //}
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
