using UnityEngine;
using System.Collections;
using UnityEditor;

//[CustomEditor(typeof(VoxelMesh))]
public class VoxelEditor : Editor
{
    [MenuItem("Window/VoxelEditor")]
    public static void ShowWindow() {
        EditorWindow.GetWindow(typeof(VoxelEditor));
    }
    public VoxelMesh voxel;

    //public SerializedObject s_voxel;
    //public SerializedProperty op,model;

    public int test = 0;

    // Use this for initialization
    void Start() {
    }

    public void OnEnable() {
        Debug.Log("VoxelEditor.OnEnable");
        //voxel = (VoxelMesh)target;
        //s_voxel = new SerializedObject(voxel);
        //op = s_voxel.FindProperty("op");
        //model = s_voxel.FindProperty("model");
        voxel.Init();
        SceneView.onSceneGUIDelegate = EditorUpdate;
    }

    public void OnDisable() {
        SceneView.onSceneGUIDelegate = null;
        Debug.Log("VoxelEditor.OnDisable");
        voxel.OnDisable();
    }

    // Update is called once per frame
    void Update() {

    }

    public override void OnInspectorGUI() {
        GUILayout.BeginHorizontal();
        if (GUILayout.Button("清空")){
            Debug.Log("clear");
            voxel.clear();
        }
        GUILayout.EndHorizontal();
        //DrawDefaultInspector();
        //SceneView.RepaintAll();
    }

    void EditorUpdate(SceneView sceneView) {
        voxel.EditorUpdate(sceneView.camera);
        SceneView.RepaintAll();
    }

}
