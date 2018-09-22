using UnityEngine;
using System.Collections;

public class ModelEditor : MonoBehaviour
{
    public GUISkin skin;
    public GUISkin browserSkin;
    public Camera modelCamera;
    public Camera uiCamera;
    public ColorDefine colorDefine;

    [HideInInspector]
    public Browser browser;
    private int designedWidth = 960;
    private int designedHeight = 640;
    private float scale = 1.0f;
    int toolWidht = 300;

    EditorTool editorTool = new EditorTool();
    CameraController cameraController = new CameraController();
    EditorCommandManager commandManager = new EditorCommandManager();
    ModelViewer modelViewer = new ModelViewer();

    private int ScreenWidth() {
        return (int)(Screen.width / scale);
    }
    private int ScreenHeight() {
        return (int)(Screen.height / scale);
    }
    void Start() {
        browser = gameObject.AddComponent<Browser>();
        browser.skin = browserSkin;
        editorTool.Init(toolWidht,ScreenWidth(),modelViewer,browser,commandManager,skin);
        cameraController.Init(modelCamera);
        modelViewer.Init(modelCamera,commandManager);
    }

    public void OnRenderObject() {
        modelViewer.OnRenderObject();
    }

    void Update() {
        modelViewer.Update();
    }

    void OnGUI() {
        //设置皮肤，缩放
        GUI.skin = skin;
        float scaleX = Screen.width / (float)designedWidth;
        float scaleY = Screen.height / (float)designedHeight;
        scale = Mathf.Min(scaleX, scaleY);

        int splitX = ScreenWidth() - toolWidht;
        modelCamera.rect = new Rect(0, 0, (splitX * 1.0f) / ScreenWidth(), 1);
        uiCamera.rect = new Rect((splitX * 1.0f) / ScreenWidth(), 0, toolWidht * 1.0f / ScreenWidth(), 1);

        //操作窗口
        if (!browser.isWindowOpened()) {
            cameraController.OnGUI();
            modelViewer.OnGUI();
        }

        GUI.matrix = Matrix4x4.TRS(Vector3.zero, Quaternion.identity, new Vector3(scale, scale, 1));
        //画工具框
        GUI.Box(new Rect(splitX, 0, toolWidht, ScreenHeight()), "");
        GUILayout.BeginArea(new Rect(splitX + 20, 20 , toolWidht - 40, ScreenHeight() - 40));
        editorTool.OnGUI();
        GUILayout.EndArea();

        if (Event.current.isKey && Event.current.keyCode == KeyCode.Z) {
            if (Input.GetKey(KeyCode.LeftControl)){
                commandManager.Undo(modelViewer.blockManager);
                modelViewer.SetDirty();
            }
        }
    }

}
