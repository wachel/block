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
            public BlockSurface(VecInt3 pos, short type, float[] ao, float[] raytraceAo,float[] extendAo)
            {
                this.pos = pos; this.type = type; smallAo = ao; this.raytraceAo = raytraceAo;
                this.extendAo = extendAo;
            }
            public VecInt3 pos;
            public short type;
            public float[] smallAo;//[4]
            public float[] raytraceAo;//[4]
            public float[] extendAo;//[4]
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
                        bool isNextOpacity = blockFun.isOpacity(chunk.getBlock(nextX, nextY, nextZ));
                        if (isCurrentVisible && !isNextOpacity) {
                            BlockSurface surface = new BlockSurface(new VecInt3(x, y, z), block, new float[4], new float[4],new float[4]);
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

        public static void SetExtendAo(List<MeshTool.BlockSurface>surface,RayCastManager scene,int normalIndex,VecInt3 gpos)
        {
            for (int i = 0; i < surface.Count; i++) {
                VecInt3 pos = surface[i].pos;
                VecInt3 adjPos = new VecInt3();
                adjPos.x = pos.x + Const.AdjacencyOffsetV[normalIndex].x + gpos.x;
                adjPos.y = pos.y + Const.AdjacencyOffsetV[normalIndex].y + gpos.y;
                adjPos.z = pos.z + Const.AdjacencyOffsetV[normalIndex].z + gpos.z;
                for (int v = 0; v < 4; v++) {
                    int curLight = scene.getLight(adjPos.x, adjPos.y, adjPos.z);
                    int num = 1;
                    for(int b = 0;b < 3; b++){
                        int sx = Const.VertexSharedOffset[normalIndex, v, b, 0];
                        int sy = Const.VertexSharedOffset[normalIndex, v, b, 1];
                        int sz = Const.VertexSharedOffset[normalIndex, v, b, 2];
                        if (!scene.testBlock(adjPos.x + sx, adjPos.y + sy, adjPos.z + sz, RayCastBlockType.Opacity)) {
                            curLight += scene.getLight(adjPos.x + sx, adjPos.y + sy, adjPos.z + sz);
                            num++;
                        }
                    }

                    surface[i].extendAo[v] = curLight / (Const.MaxLightIteration * num * 1.0f);
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
                //int x = i % ptW;
                //int y = i / ptW;
                for (int j = 0; j < 4; j++) {
                    Vector3 pos = values[i].pos + VertexOffset[normalIndex, j];
                    colors[i * 4 + j] = new Color(pos.x, pos.y, pos.z, 0);
                }
            }

            texture.SetPixels(colors);
            texture.Apply();

            return texture;
        }
        
        public static Mesh createMesh2(List<BlockSurface> surfacePoints, int normalIndex, BlockTypeFunBase blockFun)
        {
            if (surfacePoints.Count > 0) {
                ChunkMeshInfo meshInfo = new ChunkMeshInfo(surfacePoints.Count);
                for (int i = 0; i < surfacePoints.Count; i++) {
                    BlockSurface surface = surfacePoints[i];
                    meshInfo.AddSurface(surface, normalIndex, blockFun);
                }

                Mesh mesh = new Mesh();
                mesh.vertices = meshInfo.vertices.ToArray();
                mesh.triangles = meshInfo.triangles.ToArray();
                mesh.normals = meshInfo.normals.ToArray();
                //mesh.tangents = meshInfo.tangents.ToArray();
                mesh.uv = meshInfo.uv.ToArray();
                //mesh.uv2 = meshInfo.uv2.ToArray();
                mesh.colors = meshInfo.colors.ToArray();
                return mesh;
            }
            return null;
        }

        public static bool[, ,] GetVisibleBlocks(BlockChunk chunk, BlockTypeFunBase blockFun)
        {
            bool[, ,] blocks = new bool[Const.ChunkSize, Const.ChunkSize, Const.ChunkSize];
            for (int normalIndex = 0; normalIndex < 6; normalIndex++) {
                int stepX = normalIndex == 0 ? -1 : (normalIndex == 1 ? 1 : 0);
                int stepY = normalIndex == 2 ? -1 : (normalIndex == 3 ? 1 : 0);
                int stepZ = normalIndex == 4 ? -1 : (normalIndex == 5 ? 1 : 0);
                for (int x = 0; x < Const.ChunkSize; x++) {
                    for (int y = 0; y < Const.ChunkSize; y++) {
                        for (int z = 0; z < Const.ChunkSize; z++) {
                            int nextX = x + stepX;
                            int nextY = y + stepY;
                            int nextZ = z + stepZ;
                            short block = chunk.getBlock(x, y, z);
                            if (block != 0) {
                                bool isCurrentVisible = blockFun.isCollider(block);
                                short nextBlock = chunk.getBlock(nextX, nextY, nextZ);

                                bool isNextOpacity = blockFun.isCollider(nextBlock);
                                if (isCurrentVisible && !isNextOpacity) {
                                    blocks[x, y, z] = true;
                                }
                            }
                        }
                    }
                }
            }
            return blocks;
        }

        private static float getVertexSmallAo(BlockChunk chunk, BlockTypeFunBase blockFun, int x, int y, int z, int faceIndex, int vertixIndex)
        {
            float rlt = 1.0f;

            int offset0X = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 0, 0];
            int offset0Y = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 0, 1];
            int offset0Z = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 0, 2];
            bool bBlock0 = blockFun.isOpacity(chunk.getBlock(x + offset0X, y + offset0Y, z + offset0Z));

            int offset1X = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 1, 0];
            int offset1Y = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 1, 1];
            int offset1Z = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 1, 2];
            bool bBlock1 = blockFun.isOpacity(chunk.getBlock(x + offset1X, y + offset1Y, z + offset1Z));

            int offset2X = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 2, 0];
            int offset2Y = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 2, 1];
            int offset2Z = Const.VertexAdjacencyOffset[faceIndex, vertixIndex, 2, 2];
            bool bBlock2 = blockFun.isOpacity(chunk.getBlock(x + offset2X, y + offset2Y, z + offset2Z));

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
