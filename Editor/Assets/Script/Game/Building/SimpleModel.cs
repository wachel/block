using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class LayerSurfaceMesh
{
    List<Rect> rects = null;
    public LayerSurfaceMesh()
    {
        rects = new List<Rect>();
    }
    public void AddRect(Rect rect,int layer){
        rects.Add(rect);
    }
    List<Rect> GetRects(){
        return rects;
    }
}

public class Array2D<T>
{
    public T[,] data;
    public Array2D(int length0, int length1)
    {
        data = new T[length0, length1];
    }
}

public class SimpleModel 
{
    public int size = 8;
    private bool[, ,] model;
    public class Quad
    {
        public Quad(Rect rect, float depth)
        {
            this.rect = rect; this.depth = depth;
        }
        public Rect rect;
        public float ao;
        public float depth;
    }
    public List<Quad>[] quads;
    private SimpleModel() { }
    public SimpleModel(int size)
    {
        this.size = size;
        model = new bool[size, size, size];
        quads = new List<Quad>[6];
    }
    public void FillRange(VecInt3 min,VecInt3 size){
        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                for (int k = 0; k < size.z; k++) {
                    model[min.x + i, min.y + j, min.z + k] = true;
                }
            }
        }
    }
    public void ClearRange(VecInt3 min, VecInt3 size)
    {
        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                for (int k = 0; k < size.z; k++) {
                    model[i, j, k] = false;
                }
            }
        }
    }

    public void Build()
    {
        Array2D<bool>[,] layerBlocks = SplitToLayerBlocks(model);//[normal,layer]

        for (int normal = 0; normal < 6; normal++) {
            quads[normal] = new List<Quad>();
            for (int layer = 0; layer < size; layer++) {
                List<Rect> rects = GetLayerRects(layerBlocks[normal, layer].data);
                for (int i = 0; i < rects.Count; i++) {
                    quads[normal].Add(new Quad(rects[i],layer / (float)size));
                }
            }
        }

    }

    private static Array2D<bool>[,] SplitToLayerBlocks(bool[,,] model)
    {
        int size = model.GetLength(0);
        Array2D<bool>[,] layerBlocks = new Array2D<bool>[6, size];
        for (int normal = 0; normal < 6; normal++) {
            for (int layer = 0; layer < size; layer++) {
                layerBlocks[normal, layer] = new Array2D<bool>(size, size);
            }
        }
        //将体素按不同法线朝向分解成size层,每层内包含可见体素的数组
        for (int normal = 0; normal < 6; normal++) {
            int stepX = normal == 0 ? -1 : (normal == 1 ? 1 : 0);
            int stepY = normal == 2 ? -1 : (normal == 3 ? 1 : 0);
            int stepZ = normal == 4 ? -1 : (normal == 5 ? 1 : 0);
            for (int x = 0; x < size; x++) {
                for (int y = 0; y < size; y++) {
                    for (int z = 0; z < size; z++) {
                        int nextX = x + stepX;
                        int nextY = y + stepY;
                        int nextZ = z + stepZ;
                        bool isNextValid = nextX >= 0 && nextX < size && nextY >= 0 && nextY < size && nextZ >= 0 && nextZ < size;
                        bool isCurrentVisible = model[x, y, z];
                        bool isNextVisible = isNextValid ? model[nextX, nextY, nextZ] : false;
                        if (isCurrentVisible && !isNextVisible) {
                            switch (normal) {
                                case 0: layerBlocks[normal, x].data[z, y] = true; break;
                                case 1: layerBlocks[normal, size - x - 1].data[z, y] = true; break;
                                case 2: layerBlocks[normal, y].data[x, z] = true; break;
                                case 3: layerBlocks[normal, size - y - 1].data[x, z] = true; break;
                                case 4: layerBlocks[normal, z].data[x, y] = true; break;
                                case 5: layerBlocks[normal, size - z - 1].data[x, y] = true; break;
                            }
                        }
                    }
                }
            }
        }
        return layerBlocks;
    }

    private static List<Rect> GetLayerRects(bool[,] layerBlocks)
    {
        int size = layerBlocks.GetLength(0);
        List<Rect> rects = new List<Rect>();
        int[,] lineEnd = new int[size, size];//如果水平方向连续格子最后一格，则是连续格子的长度，否则为0
        for (int y = 0; y < size; y++) {
            int startX = 0;
            for (int x = 0; x <= size; x++) {
                bool cur = x == size ? false : layerBlocks[x, y];
                bool left = x == 0 ? false : layerBlocks[x - 1, y];

                if (cur && !left) {
                    startX = x;
                }
                else if (!cur && left) {
                    lineEnd[x - 1, y] = x - startX;
                }
            }
        }
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int curEndNum = lineEnd[x, y];//当前记录格子结尾数
                if (curEndNum != 0) {
                    for (int ty = y + 1; ty <= size; ty++) {
                        int nextLineEndNum = ty == size ? 0 : lineEnd[x, ty];//下一行的结尾数
                        if (nextLineEndNum != curEndNum) {
                            RectInt rectInt = new RectInt(x - curEndNum + 1, y, curEndNum, ty - y);
                            Array2DTool.SetRange(lineEnd, x, y, 1, ty - y, 0);
                            rects.Add(new Rect(rectInt.x / (float)size, rectInt.y / (float)size, rectInt.w / (float)size, rectInt.h / (float)size));
                            break;
                        }
                    }
                }
            }
        }
        return rects;
    }

}
