using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Block;
using System.Runtime.InteropServices;

class DebugTool
{
    public static void Log(System.Object message)
    {
        Debug.Log(Time.realtimeSinceStartup + ":" + message.ToString());
    }
}

public class Game
{
    public static BlockTypeFunBase blockTypeFun = new BlockTypeFun();
    public static ColorDefine colorDefine = null;

    public enum BlockType : short
    {
        Air,//默认透明块
        Out,
        Sand,//默认不透明块
        Stone,
        WhiteStone,
        Leaves,
        Log,

        WoodFloor,
        RedSand,
        WoolColoredBlack,
        StoneSlab,
        HardenedClayStainedBlue,

        StoneBrickWhiteWall0,//石砖白墙,-x朝向
        StoneBrickWhiteWall1,
        StoneBrickWhiteWall4,
        StoneBrickWhiteWall5,

        StairLeft,//-x
        StairRight,//+x
        StairBack,//-z
        StairFront,//+z

        Door_x0,
        Door_x1,
        Door_z0,
        Door_z1,


        //Stair01,  //******stairs*****
        //Stair12,  //           z
        //Stair23,  //    y     /
        //Stair30,  //    | 7-----6
        //Stair04,  //    |/|    /|
        //Stair15,  //    4-----5 |
        //Stair26,  //    | 3---|-2
        //Stair37,  //    |/    |/
        //Stair45,  //    0-----1 ----->x
        //Stair56,  //
        //Stair67,  //
        //Stair74,  //****************

        Num,
    };

}
public class BlockTypeFun : Block.BlockTypeFunBase
{
    public TexturePacker texturePacker;
    public BlockTypeFun() {
    }
    public override bool isVisible(short block)
    {
        return block != (short)BlockTypeEnum.Air;
    }
    public override bool isOpacity(short block) {
        Game.BlockType blockType = (Game.BlockType)block;
        switch (blockType) {
            case Game.BlockType.Air: return false;
            case Game.BlockType.StairLeft: return true;
            default: return true;
        }
    }
    public override bool isCollider(short block)
    {
        return block != (short)BlockTypeEnum.Air;
    }
    public override Rect getTextureUV(short block, BlockFaceIndex faceIndex) {
        return texturePacker.GetRect((Game.BlockType)block, (int)faceIndex);
    }
    public override float getFaceAo(int x, int y, int z, BlockFaceIndex faceIndex) {
        return 1.0f;
    }
    public override Color getFaceColor(short block, BlockFaceIndex faceIndex)
    {
        return TextureNameConfig.GetColor((Game.BlockType)block, (int)faceIndex);
    }
};


public class main : MonoBehaviour
{
    public TexturePacker texturePacker;
    public Texture2D packedTexture;
    public RayMarchingAo rma;
    void Start()
    {
        texturePacker = new TexturePacker();
        SimpleModelFactory.CreateAll();
        HouseItemGenerator.Init();
        StartCoroutine(InitWorld());
    }

    // Update is called once per frame
    void Update()
    {
        //Debug.Log("sssaaa");
    }

    public void OnGUI() {
        GUILayout.Label("");
        GUILayout.Label("game");
    }
    public void OnDrawGizmos()
    {
    }
    public void OnRenderObject() {
        TerrainTool.debugTool.drawAllLines();
        //if (rma != null) {
        //    Texture3D voxelTex3D = rma.GetVoxelTexture();
        //    byte[] data = new byte[1];
        //    for (int i = 0; i < data.Length; i++) {
        //        data[i] = (byte)(i % 0xff);
        //    }
        //    OpenGL.UpdateVoxel(voxelTex3D.GetNativeTexturePtr(), 0, 0, 0, 1, 1, 1, OpenGL.GL_RED, OpenGL.GL_UNSIGNED_BYTE, data, 1);
        //}
        //
        GL.IssuePluginEvent(OpenGL.GetRenderEventFunc(), 1);
        //OpenGL.TexSubImage3D(OpenGL.GL_TEXTURE_3D, 0, 0, 0, 0, 32, 32, 32, OpenGL.GL_RGBA, OpenGL.GL_UNSIGNED_BYTE, ref data);
    }

    IEnumerator InitWorld()
    {
        Block.BlockManager bm = new Block.BlockManager();

        for (int i = 0; i < (int)Game.BlockType.Num; i++) {
            for (int f = 0; f < 6; f++) {
                Game.BlockType block = (Game.BlockType)i;
                texturePacker.AddTexture(block, f, TextureNameConfig.GetTextureName(block, f));
            }
        }
        texturePacker.Pack();
        packedTexture = texturePacker.GetPackedTexture();

        BlockTypeFun blockTypeFun = new BlockTypeFun();
        blockTypeFun.texturePacker = texturePacker;
        //float startTime = Time.realtimeSinceStartup;
        int WorldSizeX = 8;
        int WorldSizeY = 2;
        int WorldSizeZ = 8;
        //申请内存
        bm.create(WorldSizeX * Const.ChunkSize, WorldSizeY * Const.ChunkSize, WorldSizeZ * Const.ChunkSize, blockTypeFun);

        DebugTool.Log("申请内存");
        yield return null;

        //
        TerrainTool.createTerrain(bm, WorldSizeX, WorldSizeY, WorldSizeZ);
        DebugTool.Log("创建地形");
        yield return null;

        TerrainTool.createTree(bm, WorldSizeX, WorldSizeY, WorldSizeZ);
        DebugTool.Log("创建树");
        yield return null;

        List<HouseItem> houseItems = new List<HouseItem>();
        TerrainTool.createBuildings(bm,ref houseItems);
        DebugTool.Log("创建建筑完成");
        yield return null;

        //光线追踪初始化
        RayCastManager rtm = new RayCastManager();
        rtm.create(bm.SizeX, bm.SizeY, bm.SizeZ);
        for (int x = 0; x < rtm.getSizeX(); x++) {
            for (int z = 0; z < rtm.getSizeZ(); z++) {
                for (int y = 0; y < rtm.getSizeY(); y++) {
                    short block = bm.getBlock(x, y, z);
                    RayCastBlockType rlt = 0;
                    rlt |= blockTypeFun.isCollider(block) ? RayCastBlockType.Collider : RayCastBlockType.Nothing;
                    rlt |= blockTypeFun.isOpacity(block) ? RayCastBlockType.Opacity : RayCastBlockType.Nothing;
                    rtm.setBlock(x, y, z, rlt);
                }
                rtm.updateInSun(x, z);
            }
        }

        //预处理光线
        Vector3[][] rays = new Vector3[6][];
        {
            for (int i = 0; i < 6; i++)
            {
                rays[i] = TerrainTool.getRandomRays(9, i);
            }
        }
        DebugTool.Log("预处理光线追踪完成");
        yield return null;

        rtm.updateAllLight();
        DebugTool.Log("扩散光照计算完毕");
        yield return null;
        //int[] lightCount = new int[Const.MaxLightIteration + 1];
        //for (int i = 0; i < WorldSizeX * Const.ChunkSize; i++) {
        //    for (int k = 0; k < WorldSizeZ * Const.ChunkSize; k++) {
        //        for (int j = 0; j < WorldSizeY * Const.ChunkSize; j++) {
        //            lightCount[rtm.getLight(i, j, k)]++;
        //        }
        //    }
        //}
        //for (int i = 0; i < lightCount.Length; i++) {
        //    Debug.Log("light (" + i + ") = " + lightCount[i]);
        //}

        Texture2D lightMap = new Texture2D(256,256,TextureFormat.RGBAHalf,false);
        Color[] colors = new Color[256 * 256];
        for(int i = 0 ;i<256; i++){
            for (int j = 0; j < 256; j++) {
                colors[j * 256 + i] = new Color(i / 256.0f, i / 256.0f, i / 256.0f, 1) * 2;
            }
        }
        lightMap.SetPixels(colors);
        lightMap.Apply();


        LightmapData[] lightmaps = new LightmapData[1];
        lightmaps[0] = new LightmapData();
        lightmaps[0].lightmapFar = lightMap;
        lightmaps[0].lightmapNear = lightMap;
        LightmapSettings.lightmaps = lightmaps;

        GameObject itemsRoot = new GameObject("items");

        for (int i = 0; i < houseItems.Count; i++) {
            HouseItem item = houseItems[i];
            string path = HouseItemGenerator.GetItemPrefabPath(item.item);
            GameObject prefab = GlobalResources.loadPrefab(path);
            if (prefab != null) {
                GameObject inst = GameObject.Instantiate<GameObject>(prefab);
                inst.name = houseItems[i].item.ToString();
                inst.transform.parent = itemsRoot.transform;
                inst.transform.position = houseItems[i].pos + new Vector3(0.5f, 0f, 0.5f);
                MeshRenderer renderer = inst.GetComponentInChildren<MeshRenderer>();
                renderer.lightmapIndex = 0;
                inst.isStatic = true;
                float light = rtm.getLight(item.pos.x, item.pos.y, item.pos.z) / (float)Const.MaxLightIteration;
                light = Mathf.Lerp(0.0f, 0.6f, light);
                renderer.lightmapScaleOffset = new Vector4(0, 0, light, 0f);
                //MeshFilter meshFilter = inst.GetComponentInChildren<MeshFilter>();
                //Mesh mesh = meshFilter.mesh;
                //Color32[] colors = new Color32[mesh.colors.Length];
                //byte c = (byte)(rtm.getLight(item.pos.x, item.pos.y, item.pos.z) * 255);
                //ArrayTool.SetAll(colors, new Color32(c,c,c,1));
                //mesh.colors32 = colors;
            }
            else {
                int a = 0;
            }
        }

        //初始化AO计算
        rma = new RayMarchingAo();
        rma.Init(rtm);

        //ViewVoxel viewVoxel = GameObject.FindObjectOfType<ViewVoxel>();
        //if (viewVoxel) {
        //    Texture3D voxelTex3D = rma.GetVoxelTexture();
        //    //OpenGLLibrary.glEnable(OpenGL.GL_TEXTURE_3D);
        //    //OpenGLLibrary.glBindTexture(OpenGL.GL_TEXTURE_3D, voxelTex3D.GetNativeTexturePtr());
        //    //byte[] data = new byte[voxelTex3D.width * voxelTex3D.height * voxelTex3D.depth * 4];
        //    //for (int i = 0; i < data.Length; i++ ){
        //    //    data[i] = 0x56;
        //    //}
        //    //OpenGL.TexSubImage3D(OpenGL.GL_TEXTURE_3D, 0, 0, 0, 0, 32, 32, 32, OpenGL.GL_RGBA, OpenGL.GL_UNSIGNED_BYTE, ref data);
        //    //OpenGL.TextureSubImage3D(voxelTex3D, 0, 0, 0, 0, 1, 1, 1, OpenGL.GL_RED, OpenGL.GL_UNSIGNED_BYTE, ref data);
        //    viewVoxel.SetVexelTex(voxelTex3D);
        //}

        //计算光照
        Vector3 sunDir = new Vector3(1, -3, 1);
        sunDir.Normalize();
        {
            //TerrainTool.calcLight(bm, rtm, rays, sunDir);
            //TerrainTool.calcLight2(bm,32);
        }
        DebugTool.Log("计算光照完成");
        yield return null;

        Material mat = GlobalResources.getBlockMaterial();
        mat.mainTexture = texturePacker.GetPackedTexture();
        GameObject root = GameObject.Find("Root");
        if(root == null) {
            root = new GameObject("Root");
        }

        Level level = GameObject.FindObjectOfType<Level>();
        if (level) {
            level.scene = rtm;
        }

        //创建网格
        for (int i = 0; i < WorldSizeX; i++)
        {
            for (int k = 0; k < WorldSizeZ; k++)
            {
                for (int j = 0; j < WorldSizeY; j++)
                {
                    int baseX = i * Const.ChunkSize;
                    int baseY = j * Const.ChunkSize;
                    int baseZ = k * Const.ChunkSize;

                    BlockChunk chunk = new BlockChunk(bm, baseX, baseY, baseZ);

                    Vector3 chunkPos = new Vector3(i * Const.ChunkSize * Const.BlockSize, j * Const.ChunkSize * Const.BlockSize, k * Const.ChunkSize * Const.BlockSize);

                    //bool[,,] visibleBlocks = new bool[Const.ChunkSize,Const.ChunkSize,Const.ChunkSize];// = MeshTool.GetVisibleBlocks(chunk, blockTypeFun);
                    //TerrainTool.calcChunkLight(chunk, i, j, k, rtm, rays, sunDir);
                    //Mesh mesh = MeshTool.createMesh(chunk, blockTypeFun, 0, 0, 0);
                    for(int f = 0; f<6; f++){
                        List<MeshTool.BlockSurface> surface = MeshTool.getChunkSurface(chunk,blockTypeFun,f);
                        //for (int s = 0; s < surface.Count; s++) {
                        //    visibleBlocks[surface[s].pos.x, surface[s].pos.y, surface[s].pos.z] = true;
                        //}


                        Texture2D texSurface = MeshTool.SurfacePointsToTexture(surface,f);

                        RenderTexture targetAoResult = rma.RenderByCalcShader(texSurface,new Vector3(i,j,k) * Const.ChunkSize,f);

                        //回读亮度数据
                        RenderTexture.active = targetAoResult;
                        Texture2D readback = new Texture2D(targetAoResult.width, targetAoResult.height);
                        readback.ReadPixels(new Rect(0, 0, targetAoResult.width, targetAoResult.width), 0, 0);
                        MeshTool.SetRaytraceAo(surface, readback);
                        MeshTool.SetExtendAo(surface, rtm, f, new VecInt3(i*Const.ChunkSize,j*Const.ChunkSize,k*Const.ChunkSize));

                        //将可行走区域标记黑色
                        //if (f == (int)BlockFaceIndex.BFI_y1) {
                        //    for (int s = 0; s < surface.Count; s++) {
                        //        int gx = baseX + surface[s].pos.x;
                        //        int gy = baseY + surface[s].pos.y;
                        //        int gz = baseZ + surface[s].pos.z;
                        //        if (rtm.testBlock(gx, gy, gz, (byte)RayTraceBlockType.Walkable)) {
                        //            for (int v = 0; v < 4; v++) {
                        //                surface[s].raytraceAo[v] *= 0.1f;
                        //            }
                        //        }
                        //    }
                        //}

                        Mesh mesh = MeshTool.createMesh2(surface,f,blockTypeFun);
                        
                        if (mesh != null) {
                            GameObject obj = new GameObject("Chunk", typeof(MeshRenderer), typeof(MeshFilter));
                            obj.isStatic = true;
                            obj.GetComponent<Renderer>().material = mat;
                            obj.GetComponent<MeshFilter>().mesh = mesh;
                            obj.transform.SetParent(root.transform);
                            obj.transform.position = chunkPos;
                        }
                    }

                    //physics.AddChunk(visibleBlocks, chunkPos);
                    yield return null;
                }
            }
        }
        StaticBatchingUtility.Combine(root);
        StaticBatchingUtility.Combine(itemsRoot);
        DebugTool.Log("生成网格");
        yield return null;
    }



};

