using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public static class TextureResource
{
    static readonly Vector3[,] VertexOffset = {
            { new Vector3(0, 0, 0), new Vector3(0, 0, 1), new Vector3(0, 1, 1), new Vector3(0, 1, 0) },//-x
            { new Vector3(1, 0, 0), new Vector3(1, 1, 0), new Vector3(1, 1, 1), new Vector3(1, 0, 1) },//+x
            { new Vector3(0, 0, 0), new Vector3(1, 0, 0), new Vector3(1, 0, 1), new Vector3(0, 0, 1) },//-y
            { new Vector3(0, 1, 0), new Vector3(0, 1, 1), new Vector3(1, 1, 1), new Vector3(1, 1, 0) },//+y
            { new Vector3(0, 0, 0), new Vector3(0, 1, 0), new Vector3(1, 1, 0), new Vector3(1, 0, 0) },//-z
            { new Vector3(0, 0, 1), new Vector3(1, 0, 1), new Vector3(1, 1, 1), new Vector3(0, 1, 1) },//+z
        };

    static readonly Vector3[] normalDefine = { 
            new Vector3(-1, 0, 0), 
            new Vector3(1, 0, 0), 
            new Vector3(0, -1, 0), 
            new Vector3(0, 1, 0), 
            new Vector3(0, 0, -1), 
            new Vector3(0, 0, 1) 
    };

    static readonly Quaternion[] rots = {   //from Vector.forward to normalDir
            Quaternion.AngleAxis(-90,Vector3.up),//-x
            Quaternion.AngleAxis( 90,Vector3.up),//+x
            Quaternion.AngleAxis( 90,Vector3.right),//-y
            Quaternion.AngleAxis(-90,Vector3.right),//+y
            Quaternion.AngleAxis(180,Vector3.up),//-z
            Quaternion.identity,//+z
    };

    public static bool[, ,] CreateVoxelData(int dx, int dy, int dz)
    {
        bool[, ,] values = new bool[dx, dy, dz];
        PerlinNoise pn = new PerlinNoise(0);

        for (int i = 0; i < dx; i++) {
            for (int k = 0; k < dz; k++) {
                for (int j = 0; j < dy; j++) {
                    values[i, j, k] = pn.Noise(i / 19.0, j / 17.0, k / 13.0) < 0;// k / 64.0f - 0.5;
                }
            }
        }
        return values;
    }

    public static Vector2[] GetRandomPoints_1_6_12()
    {
        Vector2[] rlt = new Vector2[19];
        rlt[0] = Vector2.zero;
        for (int i = 0; i < 6; i++) {
            rlt[1 + i] = Quaternion.Euler(0, 0, i * 360 / 6.0f) * new Vector2(0.4f, 0);
        }
        for (int i = 0; i < 12; i++) {
            rlt[7 + i] = Quaternion.Euler(0, 0, i * 360 / 12.0f) * new Vector2(i % 2 == 0 ? 0.9f : 0.75f, 0);
        }
        //for (int i = 0; i < 19; i++) {
        //    Debug.DrawLine(rlt[i], (Vector3)(rlt[i]) + new Vector3(0, 0, 0.1f),Color.red,100);
        //}
        return rlt;
    }

    public static Vector3[] GetDirFromPoint(Vector2[] posIn2d, int normalIndex)
    {
        Vector3[] rlt = new Vector3[posIn2d.Length];
        for (int i = 0; i < rlt.Length; i++) {
            float x = posIn2d[i].x;
            float y = posIn2d[i].y;
            float z = Mathf.Sqrt(1 - x * x - y * y);
            rlt[i] = rots[normalIndex] * new Vector3(x, y, z);
            //Debug.DrawLine(rlt[i], rlt[i] + Vector3.down * 0.1f, Color.green, 100);
        }
        return rlt;
    }

    public static Texture3D VoxelToTexture3D(bool[, ,] values)
    {
        Texture3D voxelTexture = new Texture3D(values.GetLength(0), values.GetLength(1), values.GetLength(2) / 8, TextureFormat.Alpha8, false);
        voxelTexture.filterMode = FilterMode.Point;
        voxelTexture.wrapMode = TextureWrapMode.Clamp;
        voxelTexture.SetPixels(ToColorsA(values, 8));
        voxelTexture.Apply();
        return voxelTexture;
    }

    public static Texture2D CreateConstValueTexture()
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


    public static List<Vector3> VoxelToSurfacePoints(bool[, ,] values, int baseX, int baseY, int baseZ, int dx, int dy, int dz, int normalIndex)
    {
        int stepX = normalIndex == 0 ? -1 : (normalIndex == 1 ? 1 : 0);
        int stepY = normalIndex == 2 ? -1 : (normalIndex == 3 ? 1 : 0);
        int stepZ = normalIndex == 4 ? -1 : (normalIndex == 5 ? 1 : 0);
        int lx = values.GetLength(0);
        int ly = values.GetLength(1);
        int lz = values.GetLength(2);
        List<Vector3> surfacePoints = new List<Vector3>(lx * ly * 2);
        for (int x = baseX; x < baseX + dx; x++) {
            for (int y = baseY; y < baseY + dy; y++) {
                for (int z = baseZ; z < baseZ + dz; z++) {
                    int nextX = x + stepX;
                    int nextY = y + stepY;
                    int nextZ = z + stepZ;
                    bool nextValid = nextX >= 0 && nextX < lx && nextY >= 0 && nextY < ly && nextZ >= 0 && nextZ < lz;
                    if (values[x, y, z] && (!nextValid || !values[nextX, nextY, nextZ])) {
                        surfacePoints.Add(new Vector3(x - baseX, y - baseY, z - baseZ));
                    }
                }
            }
        }
        return surfacePoints;
    }

    public static Texture2D SurfacePointsToTexture(List<Vector3> values, int normalIndex, Vector3 pos)
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
            Vector3 point0 = values[i] + (VertexOffset[normalIndex, 0] - Vector3.one * 0.5f) * 0.99f + Vector3.one * 0.5f + pos;
            Vector3 point1 = values[i] + (VertexOffset[normalIndex, 1] - Vector3.one * 0.5f) * 0.99f + Vector3.one * 0.5f + pos;
            Vector3 point2 = values[i] + (VertexOffset[normalIndex, 2] - Vector3.one * 0.5f) * 0.99f + Vector3.one * 0.5f + pos;
            Vector3 point3 = values[i] + (VertexOffset[normalIndex, 3] - Vector3.one * 0.5f) * 0.99f + Vector3.one * 0.5f + pos;

            colors[(y * 2 + 0) * (ptW * 2) + (x * 2 + 0)] = new Color(point0.x, point0.y, point0.z, 0);
            colors[(y * 2 + 0) * (ptW * 2) + (x * 2 + 1)] = new Color(point1.x, point1.y, point1.z, 0);
            colors[(y * 2 + 1) * (ptW * 2) + (x * 2 + 0)] = new Color(point2.x, point2.y, point2.z, 0);
            colors[(y * 2 + 1) * (ptW * 2) + (x * 2 + 1)] = new Color(point3.x, point3.y, point3.z, 0);
        }

        texture.SetPixels(colors);
        texture.Apply();

        return texture;
    }

    public static Mesh SufacePointsToMesh(List<Vector3> surfacePoints, int normalIndex, int lightMapWidth, int lightMapHeight)
    {
        Vector3[] vertices = new Vector3[surfacePoints.Count * 4];
        for (int i = 0; i < surfacePoints.Count; i++) {
            for (int p = 0; p < 4; p++) {
                vertices[i * 4 + p] = surfacePoints[i] + VertexOffset[normalIndex, p] - Vector3.one * 0.5f;
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
            normals[i] = normalDefine[normalIndex];
        }

        int w = lightMapWidth / 2;
        //int h = lightMapHeight / 2;

        Vector2[] uvs = new Vector2[surfacePoints.Count * 4];
        for (int i = 0; i < surfacePoints.Count; i++) {
            int x = i % w; //所在行,1表面对应2*2像素
            int y = i / w; //所在列
            Vector2 uv = new Vector2(x, y);
            Vector2 uvScale = new Vector2(1f / lightMapWidth, 1f / lightMapWidth);
            uvs[i * 4 + 0] = Vector2.Scale(uv * 2 + new Vector2(0f, 0f) + Vector2.one * 0.5f, uvScale);
            uvs[i * 4 + 1] = Vector2.Scale(uv * 2 + new Vector2(1f, 0f) + Vector2.one * 0.5f, uvScale);
            uvs[i * 4 + 2] = Vector2.Scale(uv * 2 + new Vector2(0f, 1f) + Vector2.one * 0.5f, uvScale);
            uvs[i * 4 + 3] = Vector2.Scale(uv * 2 + new Vector2(1f, 1f) + Vector2.one * 0.5f, uvScale);
        }

        Mesh mesh = new Mesh();
        mesh.vertices = vertices;
        mesh.triangles = triangles;
        mesh.normals = normals;
        mesh.uv = uvs;
        return mesh;
    }

    public static void CopyColorToMesh(Mesh mesh, Texture2D texture)
    {
        Vector2[] uvs = mesh.uv;
        Color[] pixels = texture.GetPixels();
        Color[] colors = new Color[uvs.Length];
        for (int i = 0; i < uvs.Length; i++) {
            int x = (int)(uvs[i].x * texture.width);
            int y = (int)(uvs[i].y * texture.height);
            colors[i] = pixels[y * texture.width + x];
        }
        mesh.colors = colors;
    }

    static Color[] ToColorsA(bool[, ,] values, int bits)
    {
        int dx = values.GetLength(0);
        int dy = values.GetLength(1);
        int dz = values.GetLength(2) / bits;
        Color[] colors = new Color[dx * dy * dz];
        for (int x = 0; x < dx; x++) {
            for (int y = 0; y < dy; y++) {
                for (int z = 0; z < dz; z++) {
                    int iv = 0;
                    for (int n = 0; n < bits; n++) {
                        iv |= values[x, y, z * bits + n] ? (1 << n) : 0;
                    }
                    colors[z * dx * dy + y * dx + x] = new Color(0, 0, 0, iv / (float)((1 << bits) - 1));
                }
            }
        }
        return colors;
    }
}
