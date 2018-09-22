using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class EditorTool
{
    string curPath;
    //int width = 0;
    //int height = 0;
    ModelViewer viewer;
    GUISkin skin;
    EditorCommandManager commandManager;
    Browser browser;
    string[] blockTypeNames;
    short[] blockTypeValues;
    bool bShowFileButton;
    bool bShowBuildButton;
    private void initBlockType() {
        List<string> names = new List<string>();
        List<short> values = new List<short>();
        names.Add("Delete");
        values.Add(-1);
        int index = 1;
        foreach (short myCode in Enum.GetValues(typeof(Game.BlockType))) {
            if (myCode >= (short)Game.BlockType.Sand) {
                string strName = Enum.GetName(typeof(Game.BlockType), myCode);//获取名称
                names.Add(strName);
                values.Add(myCode);
                index++;
            }
        }
        blockTypeNames = names.ToArray();
        blockTypeValues = values.ToArray();
    }
    public void Init(int w, int h,ModelViewer viewer,Browser browser,EditorCommandManager commandManager,GUISkin skin) {
        //width = w;
        //height = h;
        this.viewer = viewer;
        this.browser = browser;
        this.commandManager = commandManager;
        this.skin = skin;
        initBlockType();
    }

    public void OnGUI() {

        GUILayout.BeginHorizontal();
        if (GUILayout.Button("Undo")) {
            commandManager.Undo(viewer.blockManager);
            viewer.SetDirty();
        }
        if (GUILayout.Button("Redo")) {
            commandManager.Redo(viewer.blockManager);
            viewer.SetDirty();
        }
        GUILayout.EndHorizontal();

        GUILayout.Space(5);

        GUIBuild();
        GUIFile();
    }
    private void GUIBuild() {
        if (bShowBuildButton == false) { 
            if (GUILayout.Button("[+]Build", skin.customStyles[1])) {
                bShowBuildButton = true;
            }
        }
        else {
            if (GUILayout.Button("[-]Build", skin.customStyles[1])) {
                bShowBuildButton = false;
            }
        }

        if (bShowBuildButton) {
            int curSelectIndex = 0;
            for (int i = 0; i < blockTypeValues.Length; i++) {
                if (viewer.curOperator == ModelViewer.Operator.AddBlock && blockTypeValues[i] == (short)viewer.curBlockType) {
                    curSelectIndex = i;
                }
            }
            int selected = GUILayout.SelectionGrid(curSelectIndex, blockTypeNames, 3);
            if (blockTypeValues[selected] == -1) {
                viewer.SetOperator(ModelViewer.Operator.DeleteBlock);
            }
            else {
                viewer.SetOperator(ModelViewer.Operator.AddBlock);
                viewer.curBlockType = (Game.BlockType)blockTypeValues[selected];
            }
        }

    }

    private void GUIFile() {
        if (bShowFileButton == false) {
            if (GUILayout.Button("[+]File", skin.customStyles[1])) {
                bShowFileButton = true;
            }
        }
        else {
            if (GUILayout.Button("[-]File", skin.customStyles[1])) {
                bShowFileButton = false;
            }
        }
        if (bShowFileButton) {
            //GUILayout.BeginVertical(skin.customStyles[0]);
            GUILayout.BeginHorizontal();
            if (GUILayout.Button("New")) {
                curPath = "";
                viewer.Clear();
            }
            if (GUILayout.Button("Load")) {
                browser.OpenFile(Application.persistentDataPath, LoadModel);
            }
            GUILayout.EndHorizontal();
            GUILayout.BeginHorizontal();
            if (GUILayout.Button("Save")) {
                if (curPath != "") {
                    SaveModel(curPath);
                }
                else {
                    browser.SaveFile(Application.persistentDataPath, Application.persistentDataPath, SaveModel);
                }
            }
            if (GUILayout.Button("Save As")) {
                browser.SaveFile(Application.persistentDataPath, Application.persistentDataPath, SaveModel);
            }
            GUILayout.EndHorizontal();
            //GUILayout.BeginVertical();
        }
    }

    private void LoadModel(string file) {
        viewer.Clear();
        curPath = file;
        BlockData.Model model = ModelSerializer.LoadModel(file);
        VecInt3 offset = new VecInt3(   viewer.GetPivot().x - model.pivotX,
                                                    viewer.GetPivot().y - model.pivotY,
                                                    viewer.GetPivot().z - model.pivotZ);
        int index = 0;
        for (int i = 0; i < model.blocks.Count; i++) {
            short blockType = (short)model.blocks[i].type;
            for (int j = 0; j < model.blocks[i].lenght; j++) {
                int y = index % model.sizeY;
                int x = (index / model.sizeY) % model.sizeX;
                int z = index / (model.sizeX * model.sizeY);
                viewer.blockManager.setBlock(x + offset.x, y + offset.y, z + offset.z, blockType);
                index++;
            }
        }
        viewer.SetDirty();
        commandManager.Clear();
    }
    public void SaveModel(string file) {
        curPath = file;
        //ModelSerializer.SaveModel(viewer.blockManager,viewer.GetPivot(), file);
    }
}
