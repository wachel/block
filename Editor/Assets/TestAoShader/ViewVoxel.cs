using UnityEngine;
using System.Collections;

public class ViewVoxel : MonoBehaviour {
    GameObject[] planes;
	void Start () {
        //Texture3D tex = TextureResource.VoxelToTexture3D(TextureResource.CreateVoxelData(64, 64, 64));
        //for (int i = 0; i < 64; i++) {
        //    planes[i] = GameObject.CreatePrimitive(PrimitiveType.Quad);
        //    planes[i].transform.SetParent(transform,false);
        //    planes[i].transform.localPosition = new Vector3(0, 0, 1) * i / 64;
        //
        //    Material mat = new Material(Shader.Find("Custom/ViewVoxel2"));
        //    planes[i].GetComponent<MeshRenderer>().material = mat;
        //    mat.mainTexture = tex;
        //    mat.SetFloat("_Bit", i);
        //    mat.SetColor("_Color", Color.white * (1 - i / 64.0f));
        //}
	}

    public void SetVexelTex(Texture3D tex)
    {
        for (int i = 0; i < tex.depth; i++) {
            GameObject plane = GameObject.CreatePrimitive(PrimitiveType.Quad);
            plane.transform.SetParent(transform, false);
            plane.transform.localPosition = new Vector3(0, 0, -1) * i / tex.width;

            Material mat = new Material(Shader.Find("Custom/ViewVoxel2"));
            plane.GetComponent<MeshRenderer>().material = mat;
            mat.mainTexture = tex;
            mat.SetFloat("_Bit", i);
            mat.SetColor("_Color", Color.white * (1 - i / (float)tex.depth));
            mat.SetVector("_TexelSize", new Vector3(1.0f / tex.width, 1.0f / tex.height, 1.0f / tex.depth));

        }
    }
	
	// Update is called once per frame
	void Update () {
	
	}
}
