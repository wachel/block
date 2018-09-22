using UnityEngine;
using System.Collections;

public class test_rt : MonoBehaviour
{
    private RenderTexture m_renderTexture = null;
    private Texture m_texture = null;
    //private Texture2D m_readback = null;
    private Material m_mat = null;
    //private Mesh m_mesh = null;
    void Awake() {
        m_renderTexture = new RenderTexture(128, 128, 24,RenderTextureFormat.RFloat);
        m_renderTexture.Create();
        m_mat = new Material(Shader.Find("Custom/Perlin2D"));
        m_mat.SetFloat("Frequency", 10);
        //m_readback = new Texture2D(32,32,TextureFormat.ARGB32,false);
        //m_mesh = GLRender.createQuadMesh();
    }

    void OnGUI() {
        DrawTexture(new Rect(0,0, Screen.width, Screen.height), m_texture);
    }

    void DrawTexture(Rect rect, Texture texture) {
        RenderTexture.active = m_renderTexture;
        GLRender.Clear(true, true, Color.black);
        m_mat.SetFloat("_Frequency", 1);
        m_mat.SetFloat("_Factor", 0.0f);
        Graphics.DrawTexture(new Rect(0,0,m_renderTexture.width,m_renderTexture.height), Texture2D.whiteTexture,m_mat);
        m_mat.SetFloat("_Frequency", 4);
        m_mat.SetFloat("_Factor", 0.3f);
        Graphics.DrawTexture(new Rect(0, 0, m_renderTexture.width, m_renderTexture.height), Texture2D.whiteTexture, m_mat);
        //m_readback.ReadPixels(new Rect(0, 0, 32, 32), 0, 0);
        RenderTexture.active = null;
        //Color32[] c = m_readback.GetPixels32();
        //Color32 c0 = c[0];
        //Color32 c1 = c[1];
        //Color32 c2 = c[2];
        Graphics.DrawTexture(rect, m_renderTexture, new Rect(rect.x / Screen.width, rect.y / Screen.height, (rect.x + rect.width) / Screen.width, (rect.y + rect.height) / Screen.height), 0, 0, 0, 0);
    }


}
