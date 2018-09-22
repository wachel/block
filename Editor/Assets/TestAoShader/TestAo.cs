using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Block;

public class TestAo : MonoBehaviour
{
    //List<Color> surfacePoints = new List<Color>();
    //private Material calcAoMaterial = null;
    //public Texture2D constValueTexture;
    //public Texture3D texVoxel3D;
    //bool[, ,] voxelValues;


    public ViewVoxel viewVoxel;
    Block.BlockManager bm = new Block.BlockManager();
    Block.RayCastManager rtm;
    Block.RayMarchingAo rma;

    public Material diffuseMat;
    int numX = 2;
    int numY = 2;
    int numZ = 2;

    void Start()
    {
        diffuseMat = GlobalResources.getBlockMaterial();
    }

    public void DoGenerate()
    {
        float startTime = Time.realtimeSinceStartup;
        bm.create(numX * Const.ChunkSize, numY * Const.ChunkSize, numZ * Const.ChunkSize, new BlockTypeFun());

        for (int x = 0; x < bm.SizeX; x++) {
            for (int y = 0; y < bm.SizeY; y++) {
                for (int z = 0; z < bm.SizeZ; z++) {
                    //bm.setBlock(x, y, z, (short)(z < y ? Block.BlockTypeEnum.Air : Block.BlockTypeEnum.Sand));
                    bm.setBlock(x, y, z, (short)(y < 10 || (x == 20 && z > 10 && z <= 15) ? Block.BlockTypeEnum.Sand : Block.BlockTypeEnum.Air));
                }
            }
        }

        rtm = new Block.RayCastManager();
        rtm.create(numX, numY, numZ);
        for (int x = 0; x < bm.SizeX; x++) {
            for (int y = 0; y < bm.SizeY; y++) {
                for (int z = 0; z < bm.SizeZ; z++) {
                    rtm.setBlock(x, y, z, bm.getBlock(x, y, z) != 0 ? RayCastBlockType.All : RayCastBlockType.Nothing);
                }
            }
        }
        rma = new Block.RayMarchingAo();
        rma.Init(rtm);

        viewVoxel.SetVexelTex(rma.GetVoxelTexture());

        Debug.Log("生成完体素数据，" + (Time.realtimeSinceStartup - startTime));
        StartCoroutine(GenerateMesh());
    }

    IEnumerator GenerateMesh()
    {
        float startTime = Time.realtimeSinceStartup;
        for (int i = 0; i < numX * Const.ChunkSize; i++) {
            for (int k = 0; k < numZ * Const.ChunkSize; k++) {
                for (int j = 0; j < numY * Const.ChunkSize; j++) {
                    Block.BlockChunk chunk = new BlockChunk(bm, i * Const.ChunkSize, j * Const.ChunkSize, k * Const.ChunkSize);
                    //TerrainTool.calcChunkLight(chunk, i, j, k, rtm, rays, sunDir);
                    //Mesh mesh = MeshTool.createMesh(chunk, blockTypeFun, 0, 0, 0);
                    for (int f = 0; f < 6; f++) {
                        List<Block.MeshTool.BlockSurface> surface = Block.MeshTool.getChunkSurface(chunk, bm.blockTypeFun, f);
                        Texture2D texSurface = Block.MeshTool.SurfacePointsToTexture(surface, f);

                        RenderTexture targetAoResult = rma.RenderByCalcShader(texSurface, new Vector3(i, j, k) * Block.Const.ChunkSize, f);
                        //回读亮度数据
                        RenderTexture.active = targetAoResult;
                        Texture2D readback = new Texture2D(targetAoResult.width, targetAoResult.height);
                        readback.ReadPixels(new Rect(0, 0, targetAoResult.width, targetAoResult.width), 0, 0);
                        Block.MeshTool.SetRaytraceAo(surface, readback);
                        Mesh mesh = Block.MeshTool.createMesh2(surface, f, bm.blockTypeFun);
                        if (mesh != null) {
                            GameObject obj = new GameObject("Chunk", typeof(MeshRenderer), typeof(MeshFilter));
                            obj.isStatic = true;
                            obj.GetComponent<Renderer>().material = diffuseMat;
                            obj.GetComponent<MeshFilter>().mesh = mesh;
                            obj.transform.SetParent(transform);
                            obj.transform.position = new Vector3(i * Block.Const.ChunkSize * Block.Const.BlockSize, j * Block.Const.ChunkSize * Block.Const.BlockSize, k * Block.Const.ChunkSize * Block.Const.BlockSize);
                        }
                    }
                    yield return null;
                }
            }
        }
        StaticBatchingUtility.Combine(this.gameObject);
        Debug.Log("计算完，" + (Time.realtimeSinceStartup - startTime));
    }

    void Update()
    {
        //Graphics.SetRenderTarget(targetAoResult);
        //Graphics.Blit(texVoxel, calcAoMaterial);
        //Graphics.SetRenderTarget(null);
    }

    void Clear(RenderTexture rt)
    {
        Graphics.SetRenderTarget(rt);
        GL.Clear(true, true, new Color(0, 0, 0, 0));
        Graphics.SetRenderTarget(null);
    }

    void OnDestroy()
    {
        //Clear(targetAoResult);
    }
}
