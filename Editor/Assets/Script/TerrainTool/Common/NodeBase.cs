using UnityEngine;
using System.Collections;
using System.Xml.Serialization;
//using UnityEditor;
using System.Collections.Generic;
using System.Xml;
using System;

public enum NodeType
{
    None,
    Generator,
    UnaryOperator,
    BinaryOperator,
    TernaryOperator,
    HeightOutput,
    TextureOutput,
    TreeOutput,
    GrassOutput,
}

public enum GeneratorType
{
    Const_Value,
    Noise,
}
public enum UnaryOperatorType
{
    Curve,
    Normal,
    Erosion,
}
public enum BinaryOperatorType
{
    Add,
    Sub,
    Mul,
    Max,
}
public enum TernaryOperatorType
{
    Lerp,
}

public enum OutputType
{
    Height_Output,
}

public abstract class EditorGuiBase
{
    public abstract void labelField(string label, Color color);//EditorGUILayout.LabelField("Const_Value", style);
    public abstract void labelField(string label, int width);
    public abstract void labelField(string label);
    public abstract string textField(string label, string text);//EditorGUILayout.TextField
    public abstract float silder(string label, float value, float min, float max);//EditorGUILayout.Slider("value",value, 0f, 1f);
    public abstract bool toggle(string label,bool bValue);// EditorGUILayout.Toggle("public", bPublic);
    public abstract int intField(string label,int value);//IntField("local seed", localSeed);
    public abstract float floatField(string label, float value);
    public abstract int intSlider(string label, int value,int min ,int max);//EditorGUILayout.IntSlider
    public abstract AnimationCurve curve(string label, AnimationCurve curve);//EditorGUILayout.CurveField("curve", curve);
    public abstract void beginH();
    public abstract void beginV();
    public abstract void endH();
    public abstract void endV();
    public abstract void space();
    public abstract Enum enumPopup(string label, Enum value);
    public abstract UnityEngine.Object objectField(UnityEngine.Object obj, Type objType,bool bScene, int w, int h);
    public abstract UnityEngine.Object objectField(UnityEngine.Object obj, Type objType, bool bScene);
    public abstract bool foldout(bool bFoldout,string label);//Foldout
    public abstract void minMaxSlider(string label, ref float minValue, ref float maxValue, float minLimit, float maxLimit);
    public abstract void drawAAPolyLine(Color color, Vector3[] points);
    public abstract string getAssetPath(UnityEngine.Object obj);
}



[Serializable]
public abstract class NodeBase
{
    public NodeBase() {
        label = getDefaultName();
    }
    public string label = "name";
    public delegate NodeBase FindNodeFun(string guid);
    public string guid;
    private float[,] previewTexture;
    public string[] inputs = null;
    public abstract NodeType getNodeType();
    public abstract float[,] update(int seed,int x, int y, int w, int h,float scaleX=1.0f,float scaleY=1.0f);
    public abstract string getDefaultName();
    public float[,] getPreview() {
        return previewTexture;
    }
    public virtual void OnWindowGUI(EditorGuiBase gui){}
    public abstract void OnGUI(EditorGuiBase gui);
    public virtual void OnMainGUI(EditorGuiBase gui) { }

    private FindNodeFun funFindNode;
    public virtual GeneratorType getGeneratorType() {
        return GeneratorType.Const_Value;
    }
    public virtual UnaryOperatorType getUnaryOperatorType() {
        return UnaryOperatorType.Curve;
    }
    public virtual BinaryOperatorType getBinaryOperatorType() {
        return BinaryOperatorType.Add;
    }
    public virtual TernaryOperatorType getTernaryOperatorType() {
        return TernaryOperatorType.Lerp;
    }
    public virtual void beforeSave(){}
    public virtual void postLoaded(){}
    public void copy(NodeBase src) {
        guid = src.guid;
        label = src.label;
        funFindNode = src.funFindNode;
    }
    public void setFun(FindNodeFun funFind){
        funFindNode = funFind;
    }
    public void updatePreview(int seed, int x, int y, int w, int h) {
        previewTexture = update(seed, x, y, w, h);
    }
    public static NodeBase createNewNode(NodeType t) {
        NodeBase rlt = null;
        if (t == NodeType.Generator) {
            rlt = createNewGenerate(GeneratorType.Const_Value);
        }
        else if (t == NodeType.UnaryOperator) {
            rlt = createNewUnaryOperator(UnaryOperatorType.Curve);
        }
        else if (t == NodeType.BinaryOperator) {
            rlt = createNewBinaryOperator(BinaryOperatorType.Add);
        }
        else if (t == NodeType.TernaryOperator) {
            rlt = createNewTernaryOperator(TernaryOperatorType.Lerp);
        }
        return rlt;
    }
    public static NodeBase createNewGenerate(GeneratorType t) {
        NodeBase rlt = null;
        if (t == GeneratorType.Const_Value) {
            rlt = new NodeConst();
        }
        else if (t == GeneratorType.Noise) {
            rlt = new NodePerlin();
        }
        rlt.initInput();
        rlt.guid = Guid.NewGuid().ToString();
        return rlt;
    }
    public static NodeBase createNewUnaryOperator(UnaryOperatorType t) {
        NodeBase rlt = null;
        if (t == UnaryOperatorType.Curve) {
            rlt = new NodeCurve();
        }
        else if (t == UnaryOperatorType.Normal) {
            rlt = new NodeNormal();
        }
        else if (t == UnaryOperatorType.Erosion) {
            rlt = new NodeErosion();
        }
        rlt.initInput();
        rlt.guid = Guid.NewGuid().ToString();
        return rlt;
    }
    public static NodeBase createNewBinaryOperator(BinaryOperatorType t) {
        NodeBase rlt = null;
        rlt = new NodeBinaryOperator();
        ((NodeBinaryOperator)rlt).operatorType = t;
        rlt.initInput();
        rlt.guid = Guid.NewGuid().ToString();
        return rlt;
    }
    public static NodeBase createNewTernaryOperator(TernaryOperatorType t) {
        NodeBase rlt = null;
        rlt = new NodeTernaryOperator();
        ((NodeTernaryOperator)rlt).operatorType = t;
        rlt.initInput();
        rlt.guid = Guid.NewGuid().ToString();
        return rlt;
    }
    public static NodeBase createNewHeightOutput() {
        NodeBase rlt = new HeightOutput();
        rlt.initInput();
        rlt.guid = Guid.NewGuid().ToString();
        return rlt;
    }
    public static NodeBase createNewTextureOutput() {
        NodeBase rlt = new TextureOutput();
        rlt.initInput();
        rlt.guid = Guid.NewGuid().ToString();
        return rlt;
    }
    public static NodeBase createNewGrassOutput() {
        NodeBase rlt = new GrassOutput();
        rlt.initInput();
        rlt.guid = Guid.NewGuid().ToString();
        return rlt;
    }
    public static NodeBase createNewTreeOutput() {
        NodeBase rlt = new TreeOutput();
        rlt.initInput();
        rlt.guid = Guid.NewGuid().ToString();
        return rlt;
    }
    public override int GetHashCode() {
        int rlt = 0;
        foreach(var f in this.GetType().GetFields()){
            var val = f.GetValue(this);
            if (val != null) {
                if (f.FieldType.ToString() == "UnityEngine.AnimationCurve") {
                    AnimationCurve c = (AnimationCurve)val;
                    rlt ^= PerlinCache.getCurveHash(c);
                }
                rlt ^= val.GetHashCode();
            }
        }
        for (int i = 0; i < getInputNum(); i++) {
            if (getInputNode(i) != null) {
                rlt ^= getInputNode(i).GetHashCode();
            }
        }
        return rlt;
    }
    public virtual int getInputNum() {
        return 0;
    }
    public virtual string getInputName(int index) {
        return "";
    }
    public NodeBase getInputNode(int index) {
        return funFindNode(getInputGuid(index));
    }
    public string getInputGuid(int index) {
        return inputs[index];
    }
    public void setInputGuid(int index,string guid) {
        inputs[index] = guid;
    }
    private void initInput() {
        inputs = new string[getInputNum()];
        for (int i = 0; i < inputs.Length; i++ ) {
            inputs[i] = "";
        }
    }
    public bool hasOutput() {
        return (    getNodeType() == NodeType.Generator
                 || getNodeType() == NodeType.BinaryOperator
                 || getNodeType() == NodeType.UnaryOperator
                 || getNodeType() == NodeType.TernaryOperator
                );
    }
}

public class PerlinCache
{
    static Dictionary<string, float[,]> cachedDatas = new Dictionary<string, float[,]>();
    private static string Md5Sum(string strToEncrypt) {
        System.Text.UTF8Encoding ue = new System.Text.UTF8Encoding();
        byte[] bytes = ue.GetBytes(strToEncrypt);
        System.Security.Cryptography.MD5CryptoServiceProvider md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
        byte[] hashBytes = md5.ComputeHash(bytes);
        string hashString = "";
        for (int i = 0; i < hashBytes.Length; i++) {
            hashString += System.Convert.ToString(hashBytes[i], 16).PadLeft(2, '0');
        }
        return hashString.PadLeft(32, '0');
    }
    public static string makeKey(string model, double frequency,int octaveCount, int seed, int x, int y, int w, int h, float scaleX, float scaleY,AnimationCurve curve) {
        string temp = "";
        temp += model;
        temp += "f" + frequency.ToString();
        temp += "s" + seed.ToString();
        temp += "o" + octaveCount.ToString();
        temp += "x" + x.ToString();
        temp += "y" + y.ToString();
        temp += "w" + w.ToString();
        temp += "h" + h.ToString();
        temp += "sx" + scaleX.ToString();
        temp += "sy" + scaleY.ToString();
        temp += "c" + getCurveHash(curve).ToString();
        return Md5Sum(temp);
    }
    public static int getCurveHash(AnimationCurve val) {
        int rlt = 0;
        AnimationCurve c = (AnimationCurve)val;
        foreach (Keyframe k in c.keys) {
            rlt ^= (k.inTangent * 1.122112 + k.outTangent * 2.123123 + k.tangentMode * 0.1324123 + k.time * 4.2343113 + k.value * 2.34233).GetHashCode();
        }
        return rlt;
    }
    public static void addCache(string key, float[,] data) {
        cachedDatas.Add(key, data);
    }
    public static bool hasKey(string key) {
        return cachedDatas.ContainsKey(key);
    }
    public static float[,] getCache(string key) {
        if (cachedDatas.ContainsKey(key)) {
            return cachedDatas[key];
        }
        return null;
    }
}

[Serializable]
//for xml serialize
public class NodeWrapper:IXmlSerializable
{
    public NodeBase value;
    public NodeWrapper() {

    }
    public NodeWrapper(NodeBase node) {
        value = node;
    }
    System.Xml.Schema.XmlSchema IXmlSerializable.GetSchema() {
        return null;
    }
    void IXmlSerializable.ReadXml(XmlReader reader) {
        string rootName = reader.Name;
        string fullTypeName = string.Empty;
        string shortTypeName = string.Empty;
        XmlSerializer serial = null;

        while (reader.Read()) {
            if (reader.NodeType == XmlNodeType.Element) {
                if (reader.Name == "value") {
                    fullTypeName = reader.GetAttribute("type");
                    serial = new XmlSerializer(System.Type.GetType(fullTypeName));
                    shortTypeName = System.Type.GetType(fullTypeName).Name;
                }
                if (fullTypeName != string.Empty) {
                    if (reader.Name == shortTypeName) {
                        if (serial != null)
                            value = (NodeBase)serial.Deserialize(reader);
                    }
                }
            }
            if (reader.NodeType == XmlNodeType.EndElement) {
                if (reader.Name == rootName) {
                    reader.ReadEndElement();
                    break;
                }
            }
        }
    }

    void IXmlSerializable.WriteXml(System.Xml.XmlWriter writer) {
        writer.WriteStartElement("value");
        string strType = value.GetType().FullName;
        writer.WriteAttributeString("type", strType);
        XmlSerializer serial = new XmlSerializer(System.Type.GetType(strType));
        serial.Serialize(writer, value);
        writer.WriteEndElement();
    }
}

public class NodeConst : NodeBase
{
    public float value = 1.0f;
    public bool bPublic = false;
    private float[] texture;
    public override NodeType getNodeType() {
        return NodeType.Generator;
    }
    public override GeneratorType getGeneratorType() {
        return GeneratorType.Const_Value;
    }
    public override string getDefaultName() {
        return "generater";
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        gui.labelField("Const_Value", Color.green);
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        value = gui.silder("value",value, 0f, 1f);
        bPublic = gui.toggle("public", bPublic);
    }
    public override void OnMainGUI(EditorGuiBase gui)
    {
        if (bPublic) {
            value = gui.silder("value of " + label, value, 0f, 1f);
        }
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = new float[w,h];
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i,j] = value;
            }
        }
        return values;
    }
}



public class NodePerlin : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public float size = 100f;
    public int octaveCount = 4;
    public int localSeed = 0;
    public AnimationCurve curve = AnimationCurve.Linear(0, 0, 1,1);

    private float[] texture;
    LibNoise.Unity.Generator.Perlin generator = new LibNoise.Unity.Generator.Perlin();
    
    public override NodeType getNodeType() {
        return NodeType.Generator;
    }
    public override string getDefaultName() {
        return "generater";
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        gui.labelField("perlin noise", Color.green);
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
        localSeed = gui.intField("local seed", localSeed);
        size = gui.floatField("size", size);
        octaveCount = gui.intSlider("octaveCount", octaveCount, 1, (int)Mathf.Log(size, 2) + 2);
        curve = gui.curve("curve", curve);
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        generator.Frequency = 1.0 / size;
        generator.OctaveCount = 1;
        generator.Seed = seed + localSeed;
        generator.Quality = LibNoise.Unity.QualityMode.Low;
        //PerlinNoise perlin = new PerlinNoise(generator.Seed);

        string key = PerlinCache.makeKey("perlin", generator.Frequency, generator.Seed,octaveCount, x, y, w, h, scaleX, scaleY, curve);
        float[,] temp = null;
        if (PerlinCache.hasKey(key)) {
            temp = PerlinCache.getCache(key);
        }
        else {
            temp = new float[w, h];
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    float tempX = x + i;
                    float tempY = y + j;
                    float val = 0f;
                    float cp = 0.5f;
                    for (int o = 0; o < octaveCount; o++) {
                        float signal = (float)generator.GetValue(tempX * scaleX, tempY * scaleY, 0);
                        //float signal = (float)perlin.Noise(tempX * scaleX * generator.Frequency, tempY * scaleY * generator.Frequency, 0);
                        //float signal = 0.2f;
                        val += (curve.Evaluate(signal * 0.4f + 0.5f) - 0.5f) * 2f * cp;
                        tempX *= 1.83456789f;
                        tempY *= 1.83456789f;
                        cp *= 0.5f;
                    }
                    temp[i, j] = (val * 0.5f + 0.5f);
                }
            }
            PerlinCache.addCache(key, temp);
        }
        float[,] values = new float[w,h];
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = temp[i, j] * scale + bias;
            }
        }        
        return values;
    }
    public override GeneratorType getGeneratorType() {
        return GeneratorType.Noise;
    }
}

public class NodeCurve:NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public AnimationCurve curve = AnimationCurve.Linear(0, 0, 1, 1);
    public override NodeType getNodeType() {
        return NodeType.UnaryOperator;
    }
    public override UnaryOperatorType getUnaryOperatorType() {
        return UnaryOperatorType.Curve;
    }
    public override string getDefaultName() {
        return "curve";
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        Vector3[] points = new Vector3[41];
        for (int i = 0; i < 41; i++) {
            float x0 = i / 40.0f;
            float y0 = curve.Evaluate(x0);
            points[i].Set(x0 * 128,128 + 12 - y0 * 120,0);
        }
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
        curve = gui.curve("curve", curve);
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = new float[w, h];
        if (getInputNode(0) != null) {
            float[,] a = getInputNode(0).update(seed, x, y, w, h,scaleX,scaleY);
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    values[i, j] = curve.Evaluate(a[i,j]);
                }
            }
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = values[i, j] * scale + bias;
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 1;
    }
    public override string getInputName(int index) {
        return "i";
    }    
}

public class NodeNormal : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public float height = 100.0f;
    public enum NormalType
    {
        NT_X,
        NT_Y,
        NT_Z,
    }
    public NormalType normalType = NormalType.NT_Y;
    public override NodeType getNodeType() {
        return NodeType.UnaryOperator;
    }
    public override UnaryOperatorType getUnaryOperatorType() {
        return UnaryOperatorType.Normal;
    }
    public override string getDefaultName() {
        return "normal";
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        string[] ss = { "normal.x", "normal.y", "normal.z" };
        gui.labelField(ss[(int)normalType], Color.green);
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
        normalType = (NormalType)gui.enumPopup("normal type", normalType);
        height = gui.floatField("height", height);
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = new float[w, h];
        if (getInputNode(0) != null) {
            float[,] a = getInputNode(0).update(seed, x, y, w+1, h+1, scaleX, scaleY);
            Vector3[,] normal = new Vector3[w,h];
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    Vector3 pos_x = new Vector3(1, (a[i + 1, j] - a[i,j]) * height, 0);
                    Vector3 pos_z = new Vector3(0, (a[i, j + 1] - a[i,j]) * height, 1);
                    normal[i,j] = Vector3.Cross(pos_x,-pos_z).normalized;
                }
            }
            if (normalType == NormalType.NT_X) {
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < h; j++) {
                        values[i, j] = normal[i, j].x;
                    }
                }
            }
            else if (normalType == NormalType.NT_Z) {
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < h; j++) {
                        values[i, j] = normal[i, j].z;
                    }
                }
            }
            else if (normalType == NormalType.NT_Y) {
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < h; j++) {
                        values[i, j] = normal[i, j].y;
                    }
                }
            }
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = values[i, j] * scale + bias;
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 1;
    }
    public override string getInputName(int index) {
        return "i";
    }
}

public class NodeErosion : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public override NodeType getNodeType() {
        return NodeType.UnaryOperator;
    }
    public override UnaryOperatorType getUnaryOperatorType() {
        return UnaryOperatorType.Erosion;
    }
    public override string getDefaultName() {
        return "erosion";
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        gui.labelField("erosion", Color.green);
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = new float[w, h];
        if (getInputNode(0) != null) {
            float[,] heights = getInputNode(0).update(seed, x, y, w + 1, h + 1, scaleX, scaleY);
            Vector3[,] normal = new Vector3[w, h];
            int height = 100;
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    Vector3 pos_x = new Vector3(1, (heights[i + 1, j] - heights[i, j]) * height, 0);
                    Vector3 pos_z = new Vector3(0, (heights[i, j + 1] - heights[i, j]) * height, 1);
                    normal[i, j] = Vector3.Cross(pos_x, -pos_z).normalized;
                }
            }
            float[,] water = new float[w, h];
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    water[i, j] += 0.01f;//rain
                }
            }
            for (int i = 0; i < w - 1; i++) {
                for (int j = 0; j < h - 1; j++) {
                    water[i, j] = 0;
                }
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 1;
    }
    public override string getInputName(int index) {
        return "i";
    }
}
public class NodeBinaryOperator : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public BinaryOperatorType operatorType;
    public override NodeType getNodeType() {
        return NodeType.BinaryOperator;
    }
    public override BinaryOperatorType getBinaryOperatorType() {
        return operatorType;
    }
    public override string getDefaultName() {
        return "operator";
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        if (operatorType == BinaryOperatorType.Add) {
            gui.labelField("o = a + b", Color.green);
        }
        else if (operatorType == BinaryOperatorType.Sub) {
            gui.labelField("o = a - b", Color.green);
        }
        else if (operatorType == BinaryOperatorType.Mul) {
            gui.labelField("o = a * b", Color.green);
        }
        else if (operatorType == BinaryOperatorType.Max) {
            gui.labelField("o = max(a,b)", Color.green);
        }
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = new float[w, h];
        if (operatorType == BinaryOperatorType.Add) {
            if (getInputNode(0) != null && getInputNode(1) != null) {
                float[,] a = getInputNode(0).update(seed, x, y, w, h,scaleX,scaleY);
                float[,] b = getInputNode(1).update(seed, x, y, w, h,scaleX,scaleY);
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < h; j++) {
                        values[i, j] = a[i, j] + b[i, j];
                    }
                }
            }
        }
        else if (operatorType == BinaryOperatorType.Sub) {
            if (getInputNode(0) != null && getInputNode(1) != null) {
                float[,] a = getInputNode(0).update(seed, x, y, w, h, scaleX, scaleY);
                float[,] b = getInputNode(1).update(seed, x, y, w, h, scaleX, scaleY);
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < h; j++) {
                        values[i, j] = a[i, j] - b[i, j];
                    }
                }
            }
        }
        else if (operatorType == BinaryOperatorType.Mul) {
            if (getInputNode(0) != null && getInputNode(1) != null) {
                float[,] a = getInputNode(0).update(seed, x, y, w, h, scaleX, scaleY);
                float[,] b = getInputNode(1).update(seed, x, y, w, h, scaleX, scaleY);
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < h; j++) {
                        values[i, j] = a[i, j] * b[i, j];
                    }
                }
            }
        }
        else if (operatorType == BinaryOperatorType.Max) {
            if (getInputNode(0) != null && getInputNode(1) != null) {
                float[,] a = getInputNode(0).update(seed, x, y, w, h, scaleX, scaleY);
                float[,] b = getInputNode(1).update(seed, x, y, w, h, scaleX, scaleY);
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < h; j++) {
                        values[i, j] = a[i, j] > b[i, j] ? a[i, j] : b[i, j];
                    }
                }
            }
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = values[i, j] * scale + bias;
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 2;
    }
    public override string getInputName(int index) {
        return index == 0 ? "a" : "b";
    }
}
public class NodeTernaryOperator : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public TernaryOperatorType operatorType;
    public override NodeType getNodeType() {
        return NodeType.TernaryOperator;
    }
    public override TernaryOperatorType getTernaryOperatorType() {
        return operatorType;
    }
    public override string getDefaultName() {
        return "operator";
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        gui.labelField("lerp", Color.green);
        if (operatorType == TernaryOperatorType.Lerp) {
            gui.labelField("o = a*(1-s) + b*s",Color.green);
        }
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = new float[w, h];
        if (operatorType == TernaryOperatorType.Lerp) {
            if (getInputNode(0) != null && getInputNode(1) != null && getInputNode(2) != null) {
                float[,] a = getInputNode(0).update(seed, x, y, w, h, scaleX, scaleY);
                float[,] s = getInputNode(1).update(seed, x, y, w, h, scaleX, scaleY);
                float[,] b = getInputNode(2).update(seed, x, y, w, h, scaleX, scaleY);
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < h; j++) {
                        values[i, j] = Mathf.Lerp(a[i, j], b[i, j], s[i, j]);
                    }
                }
            }
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = values[i, j] * scale + bias;
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 3;
    }
    public override string getInputName(int index) {
        return index == 1 ? "s":(index == 0 ? "a" : "b");
    }
}
public class HeightOutput : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public override NodeType getNodeType() {
        return NodeType.HeightOutput;
    }
    public override string getDefaultName() {
        return "height";
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        gui.labelField("height output", Color.green);
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
    }
    public override void OnMainGUI(EditorGuiBase gui)
    {
        scale = gui.silder("scale for output", scale, 0f, 10f);
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = null;
        if (getInputNode(0) != null) {
            values = getInputNode(0).update(seed, x, y, w, h, scaleX, scaleY);
        }
        else {
            values = new float[w, h];
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = Mathf.Clamp(values[i, j] * scale + bias, 0f, 1f);
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 1;
    }
}

public class TextureOutput : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public int paintOrder = 0;
    public int textureIndex = 0;
    private Texture2D texture;
    public Texture2D Texture {get{return texture;}}
    public string texturePath;
    private Texture2D normal;
    public int texSizeX = 15;
    public int texSizeY = 15;
    public Texture2D Normal { get { return normal; } }
    public string normalPath;
    private bool bShowInMain = true;
    public override NodeType getNodeType() {
        return NodeType.TextureOutput;
    }
    public override string getDefaultName() {
        return "texture";
    }
    public override void beforeSave() {
        //texturePath = gui.getAssetPath(texture);
    }
    public override void postLoaded() {
        //texture = (Texture2D)UnityEditor.AssetDatabase.LoadAssetAtPath(texturePath, typeof(Texture2D));
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        gui.labelField("texture:" + paintOrder.ToString(), Color.green);
    }
    private void doTextureGUI(EditorGuiBase gui)
    {
        gui.beginH();
        {
            gui.labelField("", 10);
            gui.labelField("texture",64);
            gui.labelField("normal", 64);
            gui.labelField("size",-1);
        }
        gui.endH();
        gui.beginH();
        {
            gui.labelField("", 10);
            texture = (Texture2D)gui.objectField(texture, typeof(Texture2D), true, 64, 64);
            texturePath = gui.getAssetPath(texture);
            normal = (Texture2D)gui.objectField(normal, typeof(Texture2D), true, 64, 64);
            normalPath = gui.getAssetPath(normal);
            gui.beginV();
            {
                gui.space();
                gui.space();
                gui.beginH();
                {
                    gui.labelField("size x:", 40);
                    texSizeX = gui.intField("",texSizeX);
                }
                gui.endH();
                gui.beginH();
                {
                    gui.labelField("size y:", 40);
                    texSizeY = gui.intField("",texSizeY);
                }
                gui.endH();
            }
            gui.endV();
        }
        gui.endH();
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
        paintOrder = gui.intField("paint order", paintOrder);
        //texture = (Texture2D)EditorGUILayout.ObjectField("Texture", texture, typeof(Texture2D), true);
        //texturePath = gui.getAssetPath(texture);
        doTextureGUI(gui);
    }
    public override void OnMainGUI(EditorGuiBase gui)
    {
        bShowInMain = gui.foldout(bShowInMain, label);
        if (bShowInMain) {
            doTextureGUI(gui);
        }
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = null;
        if (getInputNode(0) != null) {
            values = getInputNode(0).update(seed, x, y, w, h, scaleX, scaleY);
        }
        else {
            values = new float[w, h];
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = Mathf.Clamp(values[i, j] * scale + bias, 0f, 1f);
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 1;
    }
}

public class GrassOutput : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public int grassIndex = 0;
    private Texture2D texture;
    public string texturePath;
    public float minSize = 1.0f;
    public float maxSize = 1.0f;
    public bool bBillboard = true;
    private bool bShowInMain = true;
    public Texture2D Texture { get { return texture; } }
  
    public override NodeType getNodeType() {
        return NodeType.GrassOutput;
    }
    public override string getDefaultName() {
        return "grass";
    }
    public override void postLoaded() {
        //texture = (Texture2D)UnityEditor.AssetDatabase.LoadAssetAtPath(texturePath, typeof(Texture2D));
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        //GUIStyle style = new GUIStyle(); style.normal.textColor = Color.green;
        gui.labelField("grass", Color.green);
    }
    private void doTextureGUI(EditorGuiBase gui)
    {
        gui.beginH();
        {
            gui.labelField("", 10);
            gui.labelField("texture", 64);
            gui.labelField("");
        }
        gui.endH();
        gui.beginH();
        {
            gui.labelField("", 10);
            texture = (Texture2D)gui.objectField(texture, typeof(Texture2D), true, 64,64);
            texturePath = gui.getAssetPath(texture);
            gui.beginV();
            {
                gui.space();
                gui.space();
                gui.beginH();
                {
                    gui.labelField("min size:", 60);
                    minSize = gui.floatField("",minSize);
                }
                gui.endH();
                gui.beginH();
                {
                    gui.labelField("max size:",60);
                    maxSize = gui.floatField("",maxSize);
                }
                gui.endH();
                gui.beginH();
                {
                    gui.labelField("billboard:", 60);
                    bBillboard = gui.toggle("",bBillboard);
                }
                gui.endH();
            }
            gui.endV();
        }
        gui.endH();
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
        //grassIndex = gui.layoutIntField("grass index", grassIndex);
        texture = (Texture2D)gui.objectField(texture, typeof(Texture2D), true);
        texturePath = gui.getAssetPath(texture);
        doTextureGUI(gui);
    }
    public override void OnMainGUI(EditorGuiBase gui)
    {
        //grassIndex = gui.layoutIntField("index of " + label, grassIndex);
        //texture = (Texture2D)EditorGUILayout.ObjectField("Texture", texture, typeof(Texture2D), true);
        //texturePath = gui.getAssetPath(texture);
        bShowInMain = gui.foldout(bShowInMain, label);
        if (bShowInMain) {
            doTextureGUI(gui);
        }
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = null;
        if (getInputNode(0) != null) {
            values = getInputNode(0).update(seed, x, y, w, h, scaleX, scaleY);
        }
        else {
            values = new float[w, h];
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = Mathf.Clamp(values[i, j] * scale + bias, 0f, 1f);
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 1;
    }
}

public class TreeOutput : NodeBase
{
    public float scale = 1.0f;
    public float bias = 0f;
    public float density = 0.1f;
    public int treeIndex = 0;
    public float bendFactor;
    public float maxSize = 1.2f;
    public float minSize = 0.8f;
    //private Texture2D texture;
    private GameObject objTree;
    public GameObject ObjTree { get { return objTree; } }
    public string treePath;
    private bool bShowInMain = true;
    public override NodeType getNodeType() {
        return NodeType.TreeOutput;
    }
    public override string getDefaultName() {
        return "tree";
    }
    public override void postLoaded() {
        //objTree = (GameObject)UnityEditor.AssetDatabase.LoadAssetAtPath(treePath, typeof(GameObject));
    }
    private void doTreeGUI(EditorGuiBase gui,bool bIndent) {
        gui.beginH();
        {
            if(bIndent){
                gui.labelField("", 10);
            }
            gui.beginV();
            {
                gui.labelField("tree");
                objTree = (GameObject)gui.objectField(objTree, typeof(GameObject), true);
                gui.minMaxSlider("size(" + minSize.ToString("0.00") + "~" + maxSize.ToString("0.00") + ")", ref minSize, ref maxSize, 0.3f, 3f);
            }
            gui.endV();
        }
        gui.endH();
        treePath = gui.getAssetPath(objTree);
    }
    public override void OnGUI(EditorGuiBase gui)
    {
        label = gui.textField("name", label);
        scale = gui.silder("scale", scale, 0f, 10f);
        bias = gui.silder("bias", bias, -5f, 5f);
        //treeIndex = gui.layoutIntField("tree index", treeIndex);
        //texture = (Texture2D)EditorGUILayout.ObjectField("Texture", texture, typeof(Texture2D), true);
        doTreeGUI(gui,false);
        density = gui.floatField("density", density);
    }
    public override void OnWindowGUI(EditorGuiBase gui)
    {
        //GUIStyle style = new GUIStyle(); style.normal.textColor = Color.green;
        gui.labelField("tree:" + treeIndex.ToString(), Color.green);
    }
    public override void OnMainGUI(EditorGuiBase gui)
    {
        //treeIndex = gui.layoutIntField("index of " + label, treeIndex);
        //texture = (Texture2D)EditorGUILayout.ObjectField(name, texture, typeof(Texture2D), true);
        bShowInMain = gui.foldout(bShowInMain, label);
        if (bShowInMain) {
            doTreeGUI(gui,true);
            gui.beginH();
            {
                //EditorGUILayout.LabelField("", GUILayout.Width(10));
                gui.labelField("", 10);
                density = gui.floatField("density of " + label, density);
            }
            gui.endH();
        }
    }
    public override float[,] update(int seed, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
        float[,] values = null;
        if (getInputNode(0) != null) {
            values = getInputNode(0).update(seed, x, y, w, h, scaleX, scaleY);
        }
        else {
            values = new float[w, h];
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                values[i, j] = Mathf.Clamp(values[i, j] * scale + bias,0f,1f);
            }
        }
        return values;
    }
    public override int getInputNum() {
        return 1;
    }
    private static long getHash(long a) {
        a = (a ^ 61) ^ (a >> 16);
        a = a + (a << 3);
        a = a ^ (a >> 4);
        a = a * 0x27d4eb2d;
        a = a ^ (a >> 15);
        return a;
    }
    public static int getTreeNum(int x,int y,float val,float density,int layer) {
        long hashCode = getHash(x*123456789+y + layer*1234567);
        float rand = (hashCode % 65536) / 65536.0f;
        float realDensity = val * density;
        int a = (int)realDensity;
        if (rand < (realDensity - a)) {
            a += 1;
        }
        return a;
    }
    public static Vector2 getTreePos(int x, int y, int index, float maxOffset, int layer) {
        long hashCode = getHash(x * 123456789 + y + index + layer * 1234567);
        float randX = ((hashCode>>8) % 65536) / 65536.0f;
        float randY = ((hashCode>>16) %65536 )/65536.0f;
        Vector2 rlt = new Vector2((randX-0.5f) * maxOffset, (randY-0.5f) * maxOffset);
        return rlt;
    }
}