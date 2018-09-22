using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace Block
{
    public class ChunkMeshInfo
    {
        public List<Vector3> vertices;
        public List<int> triangles;
        public List<Vector3> normals;
        public List<Vector4> tangents;
        public List<Vector2> uv;
        public List<Vector2> uv2;
        public List<Color> colors;

        readonly int[] indices = { 0, 1, 2, 0, 2, 3 };
        public ChunkMeshInfo()
        {
            Init(0);
        }
        public ChunkMeshInfo(int surfaceNum)
        {
            Init(surfaceNum);
        }

        private void Init(int surfaceNum){
            vertices = new List<Vector3>(surfaceNum * 4);
            triangles = new List<int>(surfaceNum * 6);
            normals = new List<Vector3>(surfaceNum * 4);
            tangents = new List<Vector4>(surfaceNum * 4);
            uv = new List<Vector2>(surfaceNum * 4);
            uv2 = new List<Vector2>(surfaceNum * 4);
            colors = new List<Color>(surfaceNum * 4);
        }

        public void AddSurface(MeshTool.BlockSurface surface, int normalIndex, BlockTypeFunBase blockFun)
        {
            if (surface.type >= (short)Game.BlockType.StairLeft && surface.type <= (short)Game.BlockType.StairFront) {
                string[] names = { "stair0", "stair1", "stair4", "stair5" };
                SimpleModel model = SimpleModelFactory.GetModel(names[surface.type - (short)Game.BlockType.StairLeft]);
                for (int i = 0; i < model.quads[normalIndex].Count; i++) {
                    SimpleModel.Quad quad = model.quads[normalIndex][i];
                    AddSubSurface(surface, normalIndex, blockFun, quad.rect, quad.depth);
                }
            }
            else{
                AddCubeSurface(surface, normalIndex, blockFun);
            }
        }

        private void AddSubSurface(MeshTool.BlockSurface surface, int normalIndex, BlockTypeFunBase blockFun, Rect rect, float depth)
        {
            int startVericesNum = vertices.Count;
            //三角形
            for (int t = 0; t < 6; t++) {
                triangles.Add(startVericesNum + indices[t]);
            }
            //顶点
            Vector3[] vs = GetSubSurfaceVertex(normalIndex,rect,depth);
            for (int p = 0; p < 4; p++) {
                Vector3 pos = surface.pos + vs[p];
                vertices.Add(pos);
                normals.Add(MeshTool.NormalDefine[normalIndex]);
                //tangents.Add(new Vector4(surface.raytraceAo[0], surface.raytraceAo[1], surface.raytraceAo[2], surface.raytraceAo[3]));
                Color color = blockFun.getFaceColor(surface.type, (Block.BlockFaceIndex)normalIndex);
                //color *= ((vs[p] - Vector3.one * 0.5f).magnitude);
                //color.a = surface.raytraceAo[p];
                color.a = (surface.extendAo[p] * 0.5f + 0.5f) * surface.raytraceAo[p];
                colors.Add(color);
            }
            //uv
            Rect uvRect = blockFun.getTextureUV(surface.type, (Block.BlockFaceIndex)normalIndex);
            uvRect = GetSubRect(uvRect, rect);
            uv.Add(new Vector2(uvRect.xMax, uvRect.yMin));
            uv.Add(new Vector2(uvRect.xMin, uvRect.yMin));
            uv.Add(new Vector2(uvRect.xMin, uvRect.yMax));
            uv.Add(new Vector2(uvRect.xMax, uvRect.yMax));

            //uv2
            uv2.Add(new Vector2(rect.xMin, rect.yMin));
            uv2.Add(new Vector2(rect.xMax, rect.yMin));
            uv2.Add(new Vector2(rect.xMax, rect.yMax));
            uv2.Add(new Vector2(rect.xMin, rect.yMax));   
        }

        private Rect GetSubRect(Rect globalRect, Rect local)
        {
            float x = globalRect.xMin + globalRect.width * local.xMin;
            float y = globalRect.yMin + globalRect.height * local.yMin;
            float w = globalRect.width * local.width;
            float h = globalRect.height * local.height;
            return new Rect(x, y, w, h);
        }

        private Vector3[] GetSubSurfaceVertex(int normalIndex,Rect rect,float depth)
        {
            float x0 = rect.xMin;
            float y0 = rect.yMin;
            float x1 = rect.xMax;
            float y1 = rect.yMax;
            float d = depth;
            float nd = 1 - d;

            switch (normalIndex) {
                case 0: return new Vector3[] { new Vector3(d, y0, x0), new Vector3(d, y0, x1), new Vector3(d, y1, x1), new Vector3(d, y1, x0) };
                case 1: return new Vector3[] { new Vector3(nd, y0, x1), new Vector3(nd, y0, x0), new Vector3(nd, y1, x0), new Vector3(nd, y1, x1) };
                case 2: return new Vector3[] { new Vector3(x0, d, y0), new Vector3(x1, d, y0), new Vector3(x1, d, y1), new Vector3(x0, d, y1) };
                case 3: return new Vector3[] { new Vector3(x1, nd, y0), new Vector3(x0, nd, y0), new Vector3(x0, nd, y1), new Vector3(x1, nd, y1) };
                case 4: return new Vector3[] { new Vector3(x1, y0, d), new Vector3(x0, y0, d), new Vector3(x0, y1, d), new Vector3(x1, y1, d) };
                default: return new Vector3[] { new Vector3(x0, y0, nd), new Vector3(x1, y0, nd), new Vector3(x1, y1, nd), new Vector3(x0, y1, nd) };
            }
        }

        private void AddCubeSurface(MeshTool.BlockSurface surface, int normalIndex, BlockTypeFunBase blockFun)
        {
            int startVericesNum = vertices.Count;
            //三角形
            for (int t = 0; t < 6; t++) {
                triangles.Add(startVericesNum + indices[t]);
            }
            //顶点
            for (int p = 0; p < 4; p++) {
                vertices.Add(surface.pos + MeshTool.VertexOffset[normalIndex, p]);
                normals.Add(MeshTool.NormalDefine[normalIndex]);
                //tangents.Add(new Vector4(surface.raytraceAo[0], surface.raytraceAo[1], surface.raytraceAo[2], surface.raytraceAo[3]));
                Color color = blockFun.getFaceColor(surface.type, (Block.BlockFaceIndex)normalIndex);
                //color.a = surface.raytraceAo[p];
                color.a = (surface.extendAo[p] * 0.8f + 0.2f) * surface.raytraceAo[p];
                colors.Add(color);
            }
            //uv
            Rect rect = blockFun.getTextureUV(surface.type, (Block.BlockFaceIndex)normalIndex);
            uv.Add(new Vector2(rect.xMax, rect.yMin));
            uv.Add(new Vector2(rect.xMin, rect.yMin));
            uv.Add(new Vector2(rect.xMin, rect.yMax));
            uv.Add(new Vector2(rect.xMax, rect.yMax));

            //uv2
            uv2.Add(new Vector2(0, 0));
            uv2.Add(new Vector2(0, 1));
            uv2.Add(new Vector2(1, 1));
            uv2.Add(new Vector2(1, 0));   
        }
    }
}