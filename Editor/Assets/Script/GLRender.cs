using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GLRender {
    public static Mesh createQuadMesh() {
        Mesh mesh = new Mesh();
        Vector3[] vertices = new Vector3[4];
        vertices[0] = new Vector3(0, 0, 0);
        vertices[1] = new Vector3(1, 0, 0);
        vertices[2] = new Vector3(1, 1, 0);
        vertices[3] = new Vector3(0, 1, 0);
        mesh.vertices = vertices;
        Vector2[] uvs = new Vector2[4];
        vertices[0] = new Vector2(0, 0);
        vertices[1] = new Vector2(1, 0);
        vertices[2] = new Vector2(1, 1);
        vertices[3] = new Vector2(0, 1);
        mesh.uv = uvs;
        int[] triangles = {0,1,2,0,2,3};
        mesh.triangles = triangles;
        return mesh;
    }
    public static Mesh createCubeMesh(float size,Color32 color) {
        Vector3[,] FaceVectors = new Vector3[,] { 
            {new Vector3(0,0,0),new Vector3(0,0,1),new Vector3(0,1,1),new Vector3(0,1,0)},
            {new Vector3(1,0,0),new Vector3(1,1,0),new Vector3(1,1,1),new Vector3(1,0,1)},
            {new Vector3(0,0,0),new Vector3(1,0,0),new Vector3(1,0,1),new Vector3(0,0,1)},
            {new Vector3(0,1,0),new Vector3(0,1,1),new Vector3(1,1,1),new Vector3(1,1,0)},
            {new Vector3(0,0,0),new Vector3(0,1,0),new Vector3(1,1,0),new Vector3(1,0,0)},
            {new Vector3(0,0,1),new Vector3(1,0,1),new Vector3(1,1,1),new Vector3(0,1,1)}
        };
        Vector3[] FaceNormals = new Vector3[]{
            new Vector3(-1,0,0),new Vector3(1,0,0),new Vector3(0,-1,0),new Vector3(0,1,0),new Vector3(0,0,-1),new Vector3(0,0,1),
        };
        Vector3[] vertices = new Vector3[24];
        Vector3[] normals = new Vector3[24];
        int[] indices = new int[36];
        for (int f = 0; f < 6; f++) {
            vertices[f * 4 + 0] = FaceVectors[f, 0];
            vertices[f * 4 + 1] = FaceVectors[f, 1];
            vertices[f * 4 + 2] = FaceVectors[f, 2];
            vertices[f * 4 + 3] = FaceVectors[f, 3];
            normals[f * 4 + 0] = normals[f * 4 + 1] = normals[f * 4 + 2] = normals[f * 4 + 3] = FaceNormals[f];
            indices[f * 6 + 0] = (short)(f * 4);
            indices[f * 6 + 1] = (short)(f * 4 + 1);
            indices[f * 6 + 2] = (short)(f * 4 + 3);
            indices[f * 6 + 3] = (short)(f * 4 + 1);
            indices[f * 6 + 4] = (short)(f * 4 + 2);
            indices[f * 6 + 5] = (short)(f * 4 + 3);
        }


        var mesh = new Mesh();
        mesh.vertices = vertices;
        mesh.triangles = indices;
        mesh.normals = normals;
        //mesh.colors32 = colors;
        //mesh.RecalculateNormals();
        return mesh;
    }
    public static void Clear(bool clearDepth, bool clearColor, Color backgroundColor) {
        GL.Clear(clearDepth, clearColor, backgroundColor);
    }
    public static void DrawQuad(Rect rect,Material mat) {
        GL.PushMatrix();
        GL.LoadOrtho();
        //GL.LoadPixelMatrix(rect.x, rect.width, rect.height, rect.y);
        mat.SetPass(0);

        GL.Begin(GL.QUADS);
        GL.Vertex3(0, 0, 0);
        GL.Vertex3(1, 0, 0);
        GL.Vertex3(1, 1, 0);
        GL.Vertex3(0, 1, 0);
        GL.End();

        GL.PopMatrix();
    }
    public static void DrawLine(Vector3 pos0, Vector3 pos1, Color color, Material mat) {
        mat.SetPass(0);
        GL.Begin(GL.LINES);
        GL.Color(color);
        GL.Vertex(pos0);
        GL.Vertex(pos1);
        GL.End();
    }
    public static void DrawLines(Vector3[] pos, Color color, Material mat) {
        mat.SetPass(0);
        GL.Begin(GL.LINES);
        GL.Color(color);
        for (int i = 1; i < pos.Length; i++) {
            GL.Vertex(pos[i - 1]);
            GL.Vertex(pos[i]);
        }
        GL.End();
    }
    public static void DrawLinePairs(Vector3[] pos, Color color, Material mat) {
        mat.SetPass(0);
        GL.Begin(GL.LINES);
        GL.Color(color);
        for (int i = 0; i < pos.Length/2; i++) {
            GL.Vertex(pos[i*2]);
            GL.Vertex(pos[i*2 + 1]);
        }
        GL.End();
    }

    public static void DrawCube(Vector3 min, Vector3 max, Color color, Material mat)
    {
        //    7--6
        //   /| /|
        //  4--5 |
        //  | 3|-2
        //  |/ |/
        //  0--1

        Vector3[] vers = { 
            new Vector3(0, 0, 0), new Vector3(0, 1, 0), new Vector3(1, 1, 0), new Vector3(1, 0, 0) ,
            new Vector3(0, 0, 1), new Vector3(0, 1, 1), new Vector3(1, 1, 1), new Vector3(1, 0, 1) 
        };
        for (int i = 0; i < 8; i++) {
            vers[i] = Vector3.Scale(vers[i], (max - min)) + min;
        }
        Vector3[] linePairs = {
                                  vers[0],vers[1],vers[1],vers[2],vers[2],vers[3],vers[3],vers[0],
                                  vers[4],vers[5],vers[5],vers[6],vers[6],vers[7],vers[7],vers[4],
                                  vers[0],vers[4],vers[1],vers[5],vers[2],vers[6],vers[3],vers[7],
                              };
        DrawLinePairs(linePairs, color, mat);
    }
    public static void DrawWireCube(Vector3 min, Vector3 max,Color color,Material mat) {
        //    7--6
        //   /| /|
        //  4--5 |
        //  | 3|-2
        //  |/ |/
        //  0--1

        Vector3[] vers = { 
            new Vector3(0, 0, 0), new Vector3(0, 1, 0), new Vector3(1, 1, 0), new Vector3(1, 0, 0) ,
            new Vector3(0, 0, 1), new Vector3(0, 1, 1), new Vector3(1, 1, 1), new Vector3(1, 0, 1) 
        };
        for (int i = 0; i < 8; i++) {
            vers[i] = Vector3.Scale(vers[i],(max - min)) + min;
        }
        Vector3[] linePairs = {
                                  vers[0],vers[1],vers[1],vers[2],vers[2],vers[3],vers[3],vers[0],
                                  vers[4],vers[5],vers[5],vers[6],vers[6],vers[7],vers[7],vers[4],
                                  vers[0],vers[4],vers[1],vers[5],vers[2],vers[6],vers[3],vers[7],
                              };
        DrawLinePairs(linePairs, color, mat);
    }

    //public static Texture2D RenderGLToTexture(int width, int height, Material material) {
    //    // get a temporary RenderTexture //
    //    RenderTexture renderTexture = RenderTexture.GetTemporary(width, height);

    //    // set the RenderTexture as global target (that means GL too)
    //    RenderTexture.active = renderTexture;

    //    // clear GL //
    //    GL.Clear(false, true, Color.black);

    //    // render GL immediately to the active render texture //
    //    RenderGLStuff(width, height, material);

    //    // read the active RenderTexture into a new Texture2D //
    //    Texture2D newTexture = new Texture2D(width, height);
    //    newTexture.ReadPixels(new Rect(0, 0, width, height), 0, 0);

    //    // apply pixels and compress //
    //    bool applyMipsmaps = false;
    //    newTexture.Apply(applyMipsmaps);
    //    bool highQuality = true;
    //    newTexture.Compress(highQuality);

    //    // clean up after the party //
    //    RenderTexture.active = null;
    //    RenderTexture.ReleaseTemporary(renderTexture);

    //    // return the goods //
    //    return newTexture;
    //}


    ////public static void RenderGLStuff(int width, int height, Material material) {
    ////    material.SetPass(0);
    ////    GL.PushMatrix();
    ////    GL.LoadPixelMatrix(0, width, height, 0);
    ////    GL.Begin(GL.LINES);
    ////    GL.Color(new Color(1, 0, 0, 0.5f));
    ////    for (int i = 0; i < 500; i++) GL.Vertex3(Random.value * width, Random.value * height, 0);
    ////    GL.End();
    ////    GL.PopMatrix();
    ////}
}
