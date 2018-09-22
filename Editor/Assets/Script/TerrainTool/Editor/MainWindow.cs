using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System;

public class MyEditorGui : EditorGuiBase
{
    public override void labelField(string label, Color color)
    {
        GUIStyle style = new GUIStyle(); style.normal.textColor = color;
        EditorGUILayout.LabelField("Const_Value", style);
    }
    public override void labelField(string label, int width)
    {
        EditorGUILayout.LabelField("Const_Value", GUILayout.Width(width));
    }
    public override void labelField(string label)
    {
        EditorGUILayout.LabelField("Const_Value");
    }
    public override string textField(string label, string text)
    {
        return EditorGUILayout.TextField(label, text);
    }
    public override float silder(string label, float value, float min, float max)
    {
        return EditorGUILayout.Slider(label, value, min, max);
    }
    public override bool toggle(string label, bool bValue)
    {
        return EditorGUILayout.Toggle(label, bValue);
    }
    public override int intField(string label, int value)
    {
        if (label == "")
        {
            return EditorGUILayout.IntField(value);
        }
        else
        {
            return EditorGUILayout.IntField(label, value);
        }
    }
    public override float floatField(string label, float value)
    {
        if (label == "")
        {
            return EditorGUILayout.FloatField(value);
        }
        else
        {
            return EditorGUILayout.FloatField(label, value);
        }
    }
    public override int intSlider(string label, int value, int min, int max)
    {
        return EditorGUILayout.IntSlider(label, value, min, max);
    }
    public override AnimationCurve curve(string label, AnimationCurve curve) { return EditorGUILayout.CurveField(label, curve); }
    public override void beginH() { EditorGUILayout.BeginHorizontal(); }
    public override void beginV() { EditorGUILayout.BeginVertical(); }
    public override void endH() { EditorGUILayout.EndHorizontal(); }
    public override void endV() { EditorGUILayout.EndVertical(); }
    public override void space() { EditorGUILayout.Space(); }
    public override Enum enumPopup(string label, Enum value) { return EditorGUILayout.EnumPopup(label, value); }
    public override UnityEngine.Object objectField(UnityEngine.Object obj, Type objType, bool bScene, int w, int h)
    {
        return EditorGUILayout.ObjectField(obj, objType, bScene, GUILayout.Width(w), GUILayout.Height(h));
    }
    public override UnityEngine.Object objectField(UnityEngine.Object obj, Type objType, bool bScene)
    {
        return EditorGUILayout.ObjectField(obj, objType, bScene);
    }
    public override bool foldout(bool bFoldout, string label)
    {
        GUIStyle myFoldoutStyle = new GUIStyle(EditorStyles.foldout);
        myFoldoutStyle.fontStyle = FontStyle.Bold;
        return EditorGUILayout.Foldout(bFoldout, label, myFoldoutStyle);
    }
    public override void minMaxSlider(string label, ref float minValue, ref float maxValue, float minLimit, float maxLimit)
    {
        //new GUIContent("size(" + minSize.ToString("0.00") + "~" + maxSize.ToString("0.00") + ")"), ref minSize, ref maxSize, 0.3f, 3f
        EditorGUILayout.MinMaxSlider(new GUIContent(label), ref minValue, ref maxValue, minLimit, maxLimit);
    }
    public override void drawAAPolyLine(Color color, Vector3[] points)
    {
        Handles.color = color;
        Handles.DrawAAPolyLine(points);
    }
    public override string getAssetPath(UnityEngine.Object obj)
    {
        return AssetDatabase.GetAssetPath(obj);
    }
}


class MainWindow : EditorWindow
{
    const string assetPath = "Assets/TerrainTool.asset";
    public string path = "";
    public float hillSize = 0.5f;
    AdvanceEditor advanceWindow = null;
    NodeManager nodeManager = null;
    public MyEditorGui gui = new MyEditorGui();
    [MenuItem("Window/TerrainTool")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(MainWindow));
    }
    void init()
    {
        minSize = new Vector2(300, 50);
        nodeManager = new NodeManager();
        NodeBase heightOutput = NodeBase.createNewHeightOutput();
        nodeManager.addNode(NodeWindow.createNew(heightOutput, new Vector2(0f, 100f), nodeManager.findNodeWindow,nodeManager.findNode));
        //AssetDatabase.CreateAsset(this, "assert");
    }

    void OnEnable()
    {
        init(); 
        NodeManager temp = NodeManager.load("temp.terr_info");
        if (temp != null) { 
            nodeManager = temp;
            postNodeMangerLoaded();
        }
        //ConfigInfo info = (ConfigInfo)AssetDatabase.LoadAssetAtPath(p, typeof(ConfigInfo));
        //nodeManager = NodeManager.createFromConfigInfo(info);
        //NodeManager temp = (NodeManager)AssetDatabase.LoadAssetAtPath(assetPath, typeof(NodeManager));
        //if (temp != null) {
        //    nodeManager = temp;
        //}
        //postNodeMangerLoaded();
    }
    void OnDisable() {
        nodeManager.save("temp.terr_info");
        //nodeManager.hideFlags = 0;
        //ConfigInfo info = nodeManager.toConfigInfo();
        //AssetDatabase.CreateAsset(nodeManager, assetPath);
    }
    void OnDestroy() {
        //string p = "Assets/TerrainTool4.asset";
        //nodeManager.hideFlags = 0;
        //ConfigInfo info = nodeManager.toConfigInfo();
        //AssetDatabase.CreateAsset(info, p);
        //AssetDatabase.SaveAssets();
    }
    void OnGUI()
    {
        GUILayout.Space(10);
        GUILayout.BeginHorizontal();
        if (GUILayout.Button("New")) { DoNew(); }
        if (GUILayout.Button("Load")) { DoLoad(); }
        if (GUILayout.Button("Save")) { DoSave(); }
        GUILayout.Space(100);
        GUILayout.EndHorizontal();

        GUILayout.Space(10);
        //hillSize = EditorGUILayout.Slider("hillSize", hillSize, 0f, 1f);
        //texture = (Texture2D)EditorGUILayout.ObjectField("Texture", texture, typeof(Texture2D), true);
        //texture2 = (Texture2D)EditorGUILayout.ObjectField("Texture", texture2, typeof(Texture2D), true);
        //tree = (Tree)EditorGUILayout.ObjectField("Tree", tree, typeof(UnityEngine.Tree), true);
        //GUILayout.FlexibleSpace();

        nodeManager.seed = EditorGUILayout.IntField("seed", nodeManager.seed);
        nodeManager.baseX = EditorGUILayout.IntField("offset.x", nodeManager.baseX);
        nodeManager.baseY = EditorGUILayout.IntField("offset.y", nodeManager.baseY);
        NodeManager.OrderValueFun orderFun = (n) => {
            if (n.node.value.getNodeType() == NodeType.HeightOutput) {
                return 0;
            }
            else if (n.node.value.getNodeType() == NodeType.TextureOutput) {
                return ((TextureOutput)n.node.value).paintOrder + 1000;
            }
            else if (n.node.value.getNodeType() == NodeType.GrassOutput) {
                return 2000;
            }
            else if (n.node.value.getNodeType() == NodeType.TreeOutput) {
                return 3000;
            }
            return 0;
        };
        nodeManager.forEachNodes_Sorted(orderFun, (n) => n.node.value.OnMainGUI(gui));

        GUILayout.Space(10);
        GUILayout.BeginHorizontal();
        if (GUILayout.Button("       Generate       ")) { DoGenerate(); }
        if (GUILayout.Button("Advance>>")) { DoAdvance(); }
        GUILayout.EndHorizontal();

        GUILayout.Space(10);


    }
    void DoNew() {
        path = "";
        init();
        postNodeMangerLoaded();
    }

    void DoLoad() {
        path = EditorUtility.OpenFilePanel("Open", "", "terr_info");
        if (path != "") {
            nodeManager = NodeManager.load(path);
            postNodeMangerLoaded();
            //string p = "Assets/TerrainTool2.asset";
            //nodeManager.hideFlags = 0;
            //nodeInfo = nodeManager.toConfigInfo();
            //AssetDatabase.CreateAsset(nodeInfo, p);
            //AssetDatabase.SaveAssets();
            //AssetDatabase.AddObjectToAsset(nodeManager.toConfigInfo(), p);
            //AssetDatabase.CreateAsset(nodeManager, "Assets/TerrainTool.asset");
        }  
    }
    void postNodeMangerLoaded() {
        //nodeManager.postLoaded();
        nodeManager.forEachNodes((n) => n.node.value.setFun(nodeManager.findNode));
        if (advanceWindow)
        {
            advanceWindow.gui = gui;
            advanceWindow.setNodeManager(nodeManager);
            advanceWindow.init();
        }
    }
    public void beforeSaveScene() {
        //nodeManager.beforeSave();
    }
    void DoSave() {
        //nodeManager.beforeSave();
        //if (path == "") {
            path = EditorUtility.SaveFilePanel("Open", "", "default", "terr_info");
        //}
        if (path != "") {
            nodeManager.save(path);
        }
    }
    void DoGenerate()
    {
        float startTime = Time.realtimeSinceStartup;
        Terrain terr = Terrain.activeTerrain;
        if (terr != null) {
            Undo.RecordObject(terr.terrainData, "generate terrain");
            nodeManager.forEachNodes((n) => {
                if (n.node.value.getNodeType() == NodeType.HeightOutput) {
                    updateTerrainHeight(n.node.value);
                }
            });
            updateTerrainTexture();
            updateTerrainGrass();
            updateTerrainTree();
        }
        float diff = Time.realtimeSinceStartup - startTime;
        Debug.Log("time = " + diff.ToString());  
    }
    private void updateTerrainHeight(NodeBase node){
        Terrain terr = Terrain.activeTerrain;
        if(terr != null){
            int w = terr.terrainData.heightmapWidth;
            int h = terr.terrainData.heightmapHeight;
            //int cx = (w-1) / 128 + 1;
            //int cy = (h-1) / 128 + 1;
            //for (int x = 0; x < cx; x++) {
            //    for (int y = 0; y < cy; y++) {
            //        int sw = (x + 1) * 128 < w ? 128 : (w - x * 128);
            //        int sh = (y + 1) * 128 < h ? 128 : (h - y * 128);
            //        float[,] values = node.update(nodeManager.seed, nodeManager.baseX + x * 128, nodeManager.baseY + y * 128, sw, sh);
            //        terr.terrainData.SetHeights(y * 128,x * 128,  values);
            //    }
            //}
            float[,] values = node.update(nodeManager.seed, nodeManager.baseX, nodeManager.baseY, w, h);
            terr.terrainData.SetHeights(0, 0, values);
        }
    }
    private void updateTerrainTexture() {
        Terrain terr = Terrain.activeTerrain;
        if (terr != null) {
            List<SplatPrototype> prototypes = new List<SplatPrototype>();
            NodeManager.OrderValueFun orderFun = (n)=>{
                if (n.node.value.getNodeType() == NodeType.TextureOutput) {
                    return ((TextureOutput)n.node.value).paintOrder;
                }
                return 0;
            };
            nodeManager.forEachNodes_Sorted(orderFun, (n) => {
                if (n.node.value.getNodeType() == NodeType.TextureOutput) {
                    TextureOutput texNode = (TextureOutput)(n.node.value);
                    if (texNode.Texture != null) {
                        texNode.textureIndex = prototypes.Count;
                        SplatPrototype p = new SplatPrototype();
                        p.texture = texNode.Texture;
                        p.normalMap = texNode.Normal;
                        p.tileSize = new Vector2(texNode.texSizeX, texNode.texSizeY);
                        prototypes.Add(p);
                    }
                }
            });
            terr.terrainData.splatPrototypes = prototypes.ToArray();
            int w = terr.terrainData.alphamapWidth;
            int h = terr.terrainData.alphamapHeight;
            float[,] totalAlpha = new float[w, h];
            float scaleX = terr.terrainData.heightmapWidth / ((float)w);
            float scaleY = terr.terrainData.heightmapHeight / ((float)w);
            int layers = terr.terrainData.alphamapLayers;
            float[, ,] alphaDatas = new float[w,h,layers];//terr.terrainData.GetAlphamaps(0, 0, w, h);
            for (int l = layers - 1; l >= 0; l--) {
                NodeBase tempNode = null;
                nodeManager.forEachNodes((n) => {
                    if (n.node.value.getNodeType() == NodeType.TextureOutput) {
                        if (((TextureOutput)n.node.value).paintOrder == l) {
                            tempNode = n.node.value;
                        }
                    }
                });
                if (tempNode != null) {
                    float[,] values = tempNode.update(nodeManager.seed, nodeManager.baseX, nodeManager.baseY, w, h, scaleX, scaleY);
                    for (int i = 0; i < w; i++) {
                        for (int j = 0; j < h; j++) {
                            if (totalAlpha[i, j] + values[i, j] <= 1.00001f) {
                                alphaDatas[i, j, l] = values[i, j];
                                totalAlpha[i, j] += values[i, j];
                            }
                            else {
                                alphaDatas[i, j, l] = 1f - totalAlpha[i, j];
                                totalAlpha[i, j] = 1f;
                            }
                        }
                    }
                }
            }
            terr.terrainData.SetAlphamaps(0, 0, alphaDatas);
        }
    }
    private void updateTerrainGrass() {
        Terrain terr = Terrain.activeTerrain;
        if (terr != null) {
            List<DetailPrototype> detailList = new List<DetailPrototype>();
            nodeManager.forEachNodes((n) => {
                if (n.node.value.getNodeType() == NodeType.GrassOutput) {
                    GrassOutput texNode = (GrassOutput)(n.node.value);
                    if (texNode.Texture != null) {
                        texNode.grassIndex = detailList.Count;
                        DetailPrototype p = new DetailPrototype();
                        p.prototypeTexture = texNode.Texture;
                        p.renderMode = texNode.bBillboard ? DetailRenderMode.GrassBillboard : DetailRenderMode.Grass;
                        detailList.Add(p);
                    }
                }
            });
            terr.terrainData.detailPrototypes = detailList.ToArray();
            int w = terr.terrainData.detailWidth;
            int h = terr.terrainData.detailHeight;
            float scaleX = terr.terrainData.heightmapWidth / ((float)w);
            float scaleY = terr.terrainData.heightmapHeight / ((float)w);
            int layers = terr.terrainData.detailPrototypes.Length;
            nodeManager.forEachNodes((n) => {
                if (n.node.value.getNodeType() == NodeType.GrassOutput) {
                    GrassOutput texNode = (GrassOutput)n.node.value;
                    float[,] values = texNode.update(nodeManager.seed, nodeManager.baseX, nodeManager.baseY, w, h, scaleX, scaleY);
                    int[,] detailValues = new int[w, h];
                    for (int i = 0; i < w; i++) {
                        for (int j = 0; j < h; j++) {
                            detailValues[i, j] = (int)(values[i, j] * values[i, j] * 16);
                        }
                    }
                    if (texNode.grassIndex < layers) {
                        terr.terrainData.SetDetailLayer(0, 0, texNode.grassIndex, detailValues);
                    }
                }
            });
        }
    }
    private void updateTerrainTree() {
        Terrain terr = Terrain.activeTerrain;
        if (terr != null) {
            List<TreePrototype> treeList = new List<TreePrototype>();
            nodeManager.forEachNodes((n) => {
                if (n.node.value.getNodeType() == NodeType.TreeOutput) {
                    TreeOutput treeNode = (TreeOutput)(n.node.value);
                    if (treeNode.ObjTree != null) {
                        treeNode.treeIndex = treeList.Count;
                        TreePrototype p = new TreePrototype();
                        p.prefab = treeNode.ObjTree;
                        p.bendFactor = treeNode.bendFactor;
                        treeList.Add(p);
                    }
                }
            });
            terr.terrainData.treePrototypes = treeList.ToArray();

            int w = terr.terrainData.heightmapWidth;
            int h = terr.terrainData.heightmapHeight;
            int layers = terr.terrainData.treePrototypes.Length;
            float pixelSize = terr.terrainData.size.x / w;
            List<TreeInstance> instances = new List<TreeInstance>();
            nodeManager.forEachNodes((n) => {
                if (n.node.value.getNodeType() == NodeType.TreeOutput) {
                    TreeOutput treeNode = (TreeOutput)n.node.value;
                    if (treeNode.treeIndex < layers) {
                        float[,] values = treeNode.update(nodeManager.seed, nodeManager.baseX, nodeManager.baseY, w, h, 1f, 1f);
                        List<Vector3> treePos = new List<Vector3>();
                        //TreePrototype prot = terr.terrainData.treePrototypes[texNode.treeIndex];
                        for (int i = 0; i < w; i++) {
                            for (int j = 0; j < h; j++) {
                                int treeNum = TreeOutput.getTreeNum(i, j, values[i, j], treeNode.density,treeNode.treeIndex);
                                for (int t = 0; t < treeNum; t++) {
                                    Vector2 offset = (TreeOutput.getTreePos(i, j, t, pixelSize * 2f, treeNode.treeIndex));
                                    Vector2 pos = new Vector2(j + offset.y, i + offset.x);//翻转x,y.
                                    float height = terr.terrainData.GetInterpolatedHeight(pos.x / w, pos.y / h) / terr.terrainData.size.y;
                                    Vector3 newPos = new Vector3(pos.x / w, height, pos.y / h);
                                    treePos.Add(newPos);
                                }
                            }
                        }
                        //
                        for (int i = 0; i < treePos.Count; i++) {
                            TreeInstance ins = new TreeInstance();
                            ins.position = treePos[i];
                            ins.prototypeIndex = treeNode.treeIndex;
                            ins.color = Color.white;
                            ins.lightmapColor = Color.white;
                            float s = UnityEngine.Random.Range(treeNode.minSize, treeNode.maxSize);
                            ins.heightScale = s;
                            ins.widthScale = s;
                            instances.Add(ins);
                        }
                    }
                }
            });
            TreeInstance[] treeInstances = new TreeInstance[instances.Count];
            for (int i = 0; i < treeInstances.Length; i++) {
                treeInstances[i] = instances[i];
            }
            terr.terrainData.treeInstances = treeInstances;
        }
    }
    void DoAdvance()
    {
        advanceWindow = (AdvanceEditor)EditorWindow.GetWindow(typeof(AdvanceEditor));
        advanceWindow.gui = gui; 
        advanceWindow.setNodeManager(nodeManager);
        advanceWindow.init();
    }
}

//public class FileModificationWarning : UnityEditor.AssetModificationProcessor
//{
//    static string[] OnWillSaveAssets(string[] paths) {
//        NodeManager.getInstance().save("temp.terr_info");
//        return paths;
//    }
//}