using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Block
{
    public class RayMarchingAo
    {
        Texture2D constValueTexture;
        Texture3D texVoxel3D;
        Material calcAoMaterial;
        public void Init(RayTraceChunkManager rtc)
        {
            constValueTexture = CreateConstValueTexture();
            texVoxel3D = Create3DTexture(rtc);
            calcAoMaterial = new Material(Shader.Find("Hidden/RaytraceAo"));


        }
        public Texture3D GetVoxelTexture()
        {
            return texVoxel3D;
        }
        public RenderTexture RenderByCalcShader(Texture2D texSurfacePoint, Vector3 startPos, int f)
        {
            RenderTexture targetAoResult = new RenderTexture(texSurfacePoint.width, texSurfacePoint.height, 0, RenderTextureFormat.ARGB32);
            targetAoResult.filterMode = FilterMode.Bilinear;
            targetAoResult.useMipMap = false;

            Graphics.SetRenderTarget(targetAoResult);

            //设置AO计算shader的输入值
            Vector3[] normalDefine = { new Vector3(-1, 0, 0), new Vector3(1, 0, 0), new Vector3(0, -1, 0), new Vector3(0, 1, 0), new Vector3(0, 0, -1), new Vector3(0, 0, 1) };
            calcAoMaterial.SetTexture("_PointTex", texSurfacePoint);
            calcAoMaterial.SetInt("_NormalIndex", f);
            calcAoMaterial.SetTexture("_ConstValueTex", constValueTexture);
            calcAoMaterial.SetVector("_Normal", normalDefine[f]);
            calcAoMaterial.SetVector("_StartPos", startPos);
            calcAoMaterial.SetVector("_TexelSize", new Vector3(1.0f / texVoxel3D.width, 1.0f / texVoxel3D.height, 1.0f / texVoxel3D.depth));

            Graphics.Blit(texVoxel3D, calcAoMaterial);
            //Graphics.SetRenderTarget(null);

            return targetAoResult;
        }

        private static Texture3D Create3DTexture(RayTraceChunkManager rtc)
        {
            Texture3D voxelTexture = new Texture3D(rtc.getSizeX(), rtc.getSizeY() / 8, rtc.getSizeZ(), TextureFormat.Alpha8, false);
            voxelTexture.filterMode = FilterMode.Point;
            voxelTexture.wrapMode = TextureWrapMode.Clamp;
            voxelTexture.SetPixels(ToColorsA(rtc, 8));
            voxelTexture.Apply();
            return voxelTexture;
        }

        private static Texture2D CreateConstValueTexture()
        {
            Texture2D texture = new Texture2D(64, 64, TextureFormat.RGBAFloat, false);
            texture.filterMode = FilterMode.Point;
            texture.wrapMode = TextureWrapMode.Clamp;

            Color[] colors = new Color[texture.width * texture.height];
            //0~5:ray dir
            for (int j = 0; j < 6; j++) {
                Vector3[] dirs = GetDirFromPoint(GetRandomPoints_1_6_12(), j);
                for (int i = 0; i < dirs.Length; i++) {
                    colors[j * texture.width + i] = new Color(dirs[i].x, dirs[i].y, dirs[i].z, 0);
                }
            }
            texture.SetPixels(colors);
            texture.Apply();
            return texture;
        }

        private static Vector2[] GetRandomPoints_1_6_12()
        {
            List<Vector2> rlt = new List<Vector2>();
            for (int i = 0; i < 4; i++) {
                rlt.Add(Quaternion.Euler(0, 0, i * 360 / 4.0f + 15) * new Vector2(0.1f, 0));
            }
            for (int i = 0; i < 6; i++) {
                rlt.Add(Quaternion.Euler(0, 0, i * 360 / 6.0f + 30) * new Vector2(0.4f, 0));
            }
            for (int i = 0; i < 12; i++) {
                rlt.Add(Quaternion.Euler(0, 0, i * 360 / 12.0f + 45) * new Vector2(i % 2 == 0 ? 0.9f : 0.75f, 0));
            }
            return rlt.ToArray();
        }

        private static Vector3[] GetDirFromPoint(Vector2[] posIn2d, int normalIndex)
        {
            Quaternion[] rots = {   //from Vector.forward to normalDir
                Quaternion.AngleAxis(-90,Vector3.up),//-x
                Quaternion.AngleAxis( 90,Vector3.up),//+x
                Quaternion.AngleAxis( 90,Vector3.right),//-y
                Quaternion.AngleAxis(-90,Vector3.right),//+y
                Quaternion.AngleAxis(180,Vector3.up),//-z
                Quaternion.identity,//+z
            };
            Vector3[] rlt = new Vector3[posIn2d.Length];
            for (int i = 0; i < rlt.Length; i++) {
                float x = posIn2d[i].x;
                float y = posIn2d[i].y;
                float z = Mathf.Sqrt(1 - x * x - y * y);
                rlt[i] = rots[normalIndex] * new Vector3(x, y, z);
            }
            return rlt;
        }

        public static Color[] ToColorsA(RayTraceChunkManager rtc, int bits)
        {
            int dcx = rtc.getChunkSize().x;
            int dcy = rtc.getChunkSize().y;
            int dcz = rtc.getChunkSize().z;
            int dgx = rtc.getSizeX();
            int dgy = rtc.getSizeY();
            int dgz = rtc.getSizeZ();
            Color[,,] colors = new Color[dgx,(dgy/bits),dgz];
            for (int cx = 0; cx < dcx; cx++) {
                for (int cy = 0; cy < dcy; cy++) {
                    for (int cz = 0; cz < dcz; cz++) {
                        RayTraceChunk chunk = rtc.getChunk(cx, cy, cz);
                        for (int bx = 0; bx < Const.ChunkSize; bx++) {
                            for (int by = 0; by < Const.ChunkSize / bits; by++) {
                                for (int bz = 0; bz < Const.ChunkSize; bz++) {
                                    int iv = 0;
                                    for (int n = 0; n < bits; n++) {
                                        iv |= chunk.testBlock(bx, by * bits + n, bz) ? (1 << n) : 0;
                                    }
                                    float a = iv / (float)((1 << bits) - 1);
                                    int gx = cx * Const.ChunkSize + bx;
                                    int gy = cy * Const.ChunkSize / bits + by;
                                    int gz = cz * Const.ChunkSize + bz;
                                    colors[gx,gy,gz] = new Color(0, 0, 0, a);
                                }
                            }
                        }
                    }
                }
            }

            int sizeX = colors.GetLength(0);
            int sizeY = colors.GetLength(1);
            int sizeZ = colors.GetLength(2);
            Color[] rlt = new Color[sizeX * sizeY * sizeZ];
            for (int x = 0; x < sizeX; x++) {
                for (int y = 0; y < sizeY; y++) {
                    for (int z = 0; z < sizeZ; z++) {
                        rlt[z * sizeY * sizeX + y * sizeX + x] = colors[x, y, z];
                    }
                }
            }
            return rlt;
        }
    }

}
