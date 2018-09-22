using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GlobalResources {
    private static Material getMaterial(ref Material mat, string name) {
        if (mat == null) {
            mat = new Material(Shader.Find(name));
        }
        return mat;
    }
    private static Material matBlock = null;
    public static Material getBlockMaterial() {
        return getMaterial(ref matBlock, "Custom/ColorDiffuse");
    }
    private static Material matAlphaBlend = null;
    public static Material getAlphaBlend() {
        return getMaterial(ref matAlphaBlend, "Mobile/Particles/Alpha Blended");
    }
    private static Material matDiffuse = null;
    public static Material getDiffuse() {
        if (matDiffuse == null) {
            matDiffuse = new Material(Shader.Find("Legacy Shaders/Diffuse"));
            matDiffuse.mainTexture = Texture2D.whiteTexture;
        }
        return matDiffuse;
    }
    private static Material matBlue = null;
    public static Material getBlue() {
        if (matBlue == null) {
            matBlue = new Material(Shader.Find("Standard"));
            matBlue.color = new Color(0, 0, 1, 0.3f);
            matBlue.SetFloat("_Mode", 3.0f);
        }
        return matBlue;
    }

    private static Dictionary<string, GameObject> prefabPool = new Dictionary<string,GameObject>();
    public static GameObject loadPrefab(string path)
    {
        if (prefabPool.ContainsKey(path)) {
            return prefabPool[path];
        }
        else {
            GameObject obj = Resources.Load<GameObject>(path);
            prefabPool[path] = obj;
            return obj;
        }
    }
}
