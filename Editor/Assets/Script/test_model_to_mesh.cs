using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Block;

public class test_model_to_mesh : MonoBehaviour
{

    public TexturePacker texturePacker;
    public Texture2D packedTexture;
    public Block.RayMarchingAo rma;

    void Start()
    {
        SimpleModelFactory.CreateAll();
        texturePacker = new TexturePacker();
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
        int WorldSizeX = 1;
        int WorldSizeY = 1;
        int WorldSizeZ = 1;
        //申请内存
        bm.create(WorldSizeX * Const.ChunkSize, WorldSizeY * Const.ChunkSize, WorldSizeZ * Const.ChunkSize, blockTypeFun);
        bm.setBlock(3, 4, 4, (int)Game.BlockType.StairLeft);
        bm.setBlock(5, 4, 4, (int)Game.BlockType.StairRight);
        bm.setBlock(4, 4, 3, (int)Game.BlockType.StairBack);
        bm.setBlock(4, 4, 5, (int)Game.BlockType.StairFront);

        //光线追踪初始化
        Block.RayCastManager rtm = new Block.RayCastManager();
        rtm.create(bm.SizeX, bm.SizeY, bm.SizeZ);
        for (int x = 0; x < bm.SizeX; x++) {
            for (int y = 0; y < bm.SizeY; y++) {
                for (int z = 0; z < bm.SizeZ; z++) {
                    rtm.setBlock(x, y, z, bm.getBlock(x, y, z) != 0 ? RayCastBlockType.All : RayCastBlockType.Nothing);
                }
            }
        }


        //初始化AO计算
        rma = new Block.RayMarchingAo();
        rma.Init(rtm);
        ViewVoxel viewVoxel = GameObject.FindObjectOfType<ViewVoxel>();
        if (viewVoxel) {
            Texture3D voxelTex3D = rma.GetVoxelTexture();
            viewVoxel.SetVexelTex(voxelTex3D);
        }

        Material mat = GlobalResources.getBlockMaterial();
        mat.mainTexture = texturePacker.GetPackedTexture();
        GameObject root = this.gameObject;
        if (root == null) {
            root = new GameObject("Root");
        }

        //创建网格
        for (int i = 0; i < WorldSizeX; i++) {
            for (int k = 0; k < WorldSizeZ; k++) {
                for (int j = 0; j < WorldSizeY; j++) {
                    Block.BlockChunk chunk = new BlockChunk(bm, i * Const.ChunkSize, j * Const.ChunkSize, k * Const.ChunkSize);
                    //TerrainTool.calcChunkLight(chunk, i, j, k, rtm, rays, sunDir);
                    //Mesh mesh = MeshTool.createMesh(chunk, blockTypeFun, 0, 0, 0);
                    for (int f = 0; f < 6; f++) {
                        List<Block.MeshTool.BlockSurface> surface = Block.MeshTool.getChunkSurface(chunk, blockTypeFun, f);
                        Texture2D texSurface = Block.MeshTool.SurfacePointsToTexture(surface, f);

                        RenderTexture targetAoResult = rma.RenderByCalcShader(texSurface, new Vector3(i, j, k) * Block.Const.ChunkSize, f);

                        //回读亮度数据
                        RenderTexture.active = targetAoResult;
                        Texture2D readback = new Texture2D(targetAoResult.width, targetAoResult.height);
                        readback.ReadPixels(new Rect(0, 0, targetAoResult.width, targetAoResult.width), 0, 0);
                        Block.MeshTool.SetRaytraceAo(surface, readback);
                        Mesh mesh = Block.MeshTool.createMesh2(surface, f, blockTypeFun);

                        if (mesh != null) {
                            GameObject obj = new GameObject("Chunk", typeof(MeshRenderer), typeof(MeshFilter));
                            obj.isStatic = true;
                            obj.GetComponent<Renderer>().material = mat;
                            obj.GetComponent<MeshFilter>().mesh = mesh;
                            obj.transform.SetParent(root.transform);
                            obj.transform.position = new Vector3(i * Block.Const.ChunkSize * Block.Const.BlockSize, j * Block.Const.ChunkSize * Block.Const.BlockSize, k * Block.Const.ChunkSize * Block.Const.BlockSize);
                        }
                    }
                }
            }
        }

    }

    // Update is called once per frame
    void Update()
    {

    }
}
