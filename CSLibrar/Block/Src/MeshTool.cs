using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace Block
{
    public static class MeshTool
    {
        public static readonly Vector3[,] VertexOffset = {
            { new Vector3(0, 0, 0), new Vector3(0, 0, 1), new Vector3(0, 1, 1), new Vector3(0, 1, 0) },//-x
            { new Vector3(1, 0, 1), new Vector3(1, 0, 0), new Vector3(1, 1, 0), new Vector3(1, 1, 1) },//+x

            { new Vector3(0, 0, 0), new Vector3(1, 0, 0), new Vector3(1, 0, 1), new Vector3(0, 0, 1) },//-y
            { new Vector3(1, 1, 0), new Vector3(0, 1, 0), new Vector3(0, 1, 1), new Vector3(1, 1, 1) },//+y

            { new Vector3(1, 0, 0), new Vector3(0, 0, 0), new Vector3(0, 1, 0), new Vector3(1, 1, 0) },//-z
            { new Vector3(0, 0, 1), new Vector3(1, 0, 1), new Vector3(1, 1, 1), new Vector3(0, 1, 1) },//+z
        };

        public static readonly Vector3[] NormalDefine = { 
            new Vector3(-1, 0, 0), 
            new Vector3(1, 0, 0), 
            new Vector3(0, -1, 0), 
            new Vector3(0, 1, 0), 
            new Vector3(0, 0, -1), 
            new Vector3(0, 0, 1) 
        };

        public struct BlockSurface{
            public BlockSurface(Vector3 pos, short type, float[] ao,float[] raytraceAo) {
                this.pos = pos; this.type = type; smallAo = ao; this.raytraceAo = raytraceAo;
            }
            public Vector3 pos;
            public short type;
            public float[] smallAo;
            public float[] raytraceAo;
        }

        public static List<BlockSurface> getChunkSurface(BlockChunk chunk, BlockTypeFunBase blockFun, int normalIndex)
        {
            int stepX = normalIndex == 0 ? -1 : (normalIndex == 1 ? 1 : 0);
            int stepY = normalIndex == 2 ? -1 : (normalIndex == 3 ? 1 : 0);
            int stepZ = normalIndex == 4 ? -1 : (normalIndex == 5 ? 1 : 0);
            List<BlockSurface> surfacePoints = new List<BlockSurface>();
            for (int x = 0; x < Const.ChunkSize; x++) {
                for (int y = 0; y < Const.ChunkSize; y++) {
                    for (int z = 0; z < Const.ChunkSize; z++) {
                        int nextX = x + stepX;
                        int nextY = y + stepY;
                        int nextZ = z + stepZ;
                        short block = chunk.getBlock(x, y, z);
                        bool isCurrentVisible = blockFun.isVisible(block);
                        bool isNextOpacity = blockFun.isOpacity(chunk.getBlockSmart(nextX, nextY, nextZ));
                        if (isCurrentVisible && !isNextOpacity) {
                            BlockSurface surface = new BlockSurface(new Vector3(x, y, z), block, new float[4],new float[4]);
                            for (int v = 0; v < 4; v++) {
                                surface.smallAo[v] = getVertexSmallAo(chunk, blockFun, x, y, z, normalIndex, v);
                            }
                            surfacePoints.Add(surface);
                        }
                    }
                }
            }
            return surfacePoints;
        }

        public static void SetRaytraceAo(List<MeshTool.BlockSurface> surface, Texture2D aoResult)
        {                
            if (aoResult) {
                Color[] aoColors = aoResult.GetPixels();
                for (int i = 0; i < surface.Count; i++) {
                    for(int v = 0; v<4; v++){
                        surface[i].raytraceAo[v] = aoColors[i * 4 + v].a;
                    }
                }
            }
        }

        public static Texture2D SurfacePointsToTexture(List<BlockSurface> values, int normalIndex)
        {
            int ptW = 1;
            while (ptW * ptW < values.Count) {
                ptW *= 2;
            }
            int ptH = ptW;
            if (ptW * ptH <= values.Count / 2) {
                ptH /= 2;
            }

            Texture2D texture = new Texture2D(ptW * 2, ptH * 2, TextureFormat.RGBAFloat, false);//1表面对应4像素
            texture.filterMode = FilterMode.Point;
            Color[] colors = new Color[ptW * ptH * 4];
            for (int i = 0; i < values.Count; i++) {
                int x = i % ptW;
                int y = i / ptW;
                for (int j = 0; j < 4; j++) {
                    Vector3 pos = values[i].pos + VertexOffset[normalIndex, j];
                    colors[i * 4 + j] = new Color(pos.x, pos.y, pos.z, 0);
                }
            }

            texture.SetPixels(colors);
            texture.Apply();

            return texture;
        }

        public static Mesh createMesh(List<BlockSurface> surfacePoints,int normalIndex,Texture2D aoTexture, BlockTypeFunBase blockFun)
        {
            if (surfacePoints.Count > 0) {
                Vector3[] vertices = new Vector3[surfacePoints.Count * 4];
                for (int i = 0; i < surfacePoints.Count; i++) {
                    for (int p = 0; p < 4; p++) {
                        vertices[i * 4 + p] = surfacePoints[i].pos + VertexOffset[normalIndex, p];
                    }
                }
                int[] triangles = new int[surfacePoints.Count * 6];
                for (int i = 0; i < surfacePoints.Count; i++) {
                    triangles[i * 6 + 0] = i * 4 + 0;
                    triangles[i * 6 + 1] = i * 4 + 1;
                    triangles[i * 6 + 2] = i * 4 + 2;
                    triangles[i * 6 + 3] = i * 4 + 0;
                    triangles[i * 6 + 4] = i * 4 + 2;
                    triangles[i * 6 + 5] = i * 4 + 3;
                }

                Vector3[] normals = new Vector3[vertices.Length];
                for (int i = 0; i < normals.Length; i++) {
                    normals[i] = NormalDefine[normalIndex];
                }

                Vector4[] tangents = new Vector4[surfacePoints.Count * 4];
                if (aoTexture) {
                    Color[] aoColors = aoTexture.GetPixels();
                    for (int i = 0; i < surfacePoints.Count; i++) {
                        for (int v = 0; v < 4; v++) {
                            tangents[i * 4 + v] = new Vector4(aoColors[i * 4 + 0].a, aoColors[i * 4 + 1].a, aoColors[i * 4 + 2].a, aoColors[i * 4 + 3].a);
                        }
                    }
                }

                Vector2[] uv = new Vector2[surfacePoints.Count * 4];
                for (int i = 0; i < surfacePoints.Count; i++)
                {
                    Rect rect = blockFun.getTextureUV(surfacePoints[i].type, (Block.BlockFaceIndex)normalIndex);
                    uv[i * 4 + 0] = new Vector2(rect.xMax, rect.yMin);
                    uv[i * 4 + 1] = new Vector2(rect.xMin, rect.yMin);
                    uv[i * 4 + 2] = new Vector2(rect.xMin, rect.yMax);
                    uv[i * 4 + 3] = new Vector2(rect.xMax, rect.yMax);
                }

                Vector2[] uv2 = new Vector2[surfacePoints.Count * 4];
                for (int i = 0; i < surfacePoints.Count; i++)
                {
                    uv2[i * 4 + 0] = new Vector2(0, 0);
                    uv2[i * 4 + 1] = new Vector2(0, 1);
                    uv2[i * 4 + 2] = new Vector2(1, 1);
                    uv2[i * 4 + 3] = new Vector2(1, 0);
                }

                Color[] colors = new Color[surfacePoints.Count * 4];
                for (int i = 0; i < surfacePoints.Count; i++) {
                    for (int v = 0; v < 4; v++) {
                        colors[i*4 + v] = blockFun.getFaceColor(surfacePoints[i].type, (Block.BlockFaceIndex)normalIndex);
                    }
                }

                Mesh mesh = new Mesh();
                mesh.vertices = vertices;
                mesh.triangles = triangles;
                mesh.normals = normals;
                mesh.tangents = tangents;
                mesh.uv = uv;
                mesh.uv2 = uv2;
                mesh.colors = colors;
                return mesh;
            }
            return null;
        }


        public static Mesh createMesh2(List<BlockSurface> surfacePoints, int normalIndex, BlockTypeFunBase blockFun,ChunkMeshInfoBase chunkMeshInfo)
        {
            if (surfacePoints.Count > 0) {
                ChunkMeshInfoBase meshInfo = new ChunkMeshInfo(surfacePoints.Count);
                for (int i = 0; i < surfacePoints.Count; i++) {
                    BlockSurface surface = surfacePoints[i];
                    meshInfo.AddSurface(surface, normalIndex, blockFun);
                }

                Mesh mesh = new Mesh();
                mesh.vertices = meshInfo.vertices.ToArray();
                mesh.triangles = meshInfo.triangles.ToArray();
                mesh.normals = meshInfo.normals.ToArray();
                mesh.tangents = meshInfo.tangents.ToArray();
                mesh.uv = meshInfo.uv.ToArray();
                mesh.uv2 = meshInfo.uv2.ToArray();
                mesh.colors = meshInfo.colors.ToArray();
                return mesh;
            }
            return null;
        }

        private static void AddCube()
        {

        }

        private static float getVertexSmallAo(BlockChunk blocks, BlockTypeFunBase blockFun, int x, int y, int z, int faceIndex, int vertixIndex)
        {
            float rlt = 1.0f;

            int offset0X = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 0, 0];
            int offset0Y = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 0, 1];
            int offset0Z = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 0, 2];
            bool bBlock0 = blockFun.isOpacity(blocks.getBlockSmart(x + offset0X, y + offset0Y, z + offset0Z));

            int offset1X = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 1, 0];
            int offset1Y = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 1, 1];
            int offset1Z = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 1, 2];
            bool bBlock1 = blockFun.isOpacity(blocks.getBlockSmart(x + offset1X, y + offset1Y, z + offset1Z));

            int offset2X = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 2, 0];
            int offset2Y = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 2, 1];
            int offset2Z = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 2, 2];
            bool bBlock2 = blockFun.isOpacity(blocks.getBlockSmart(x + offset2X, y + offset2Y, z + offset2Z));

            if (bBlock0 && bBlock2) {//如果两个共边对角块都是实体，则共点对角块存不存在已经无所谓，说明是三面内角
                rlt *= 0.7f;
            }
            else if ((bBlock0 && bBlock1) || (bBlock1 && bBlock2)) {//如果有一个共边对角块和一个共点对角块，则说明是阶梯的内角
                rlt *= 0.8f;
            }
            else if (bBlock0 || bBlock1 || bBlock2) {
                rlt *= 0.8f;
            }
            else {
                rlt *= 1.0f;
            }
            return rlt;
        }

    };

}
