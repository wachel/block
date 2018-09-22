using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public static class TextureNameConfig
{
    public static string GetTextureName(Game.BlockType block,int faceIndex)
    {
        switch (block) {
            case Game.BlockType.Sand        : return "sand";
            case Game.BlockType.Stone       : return "stone";
            case Game.BlockType.WhiteStone  : return "white_stone";
            case Game.BlockType.Log         : return faceIndex ==2 || faceIndex == 3 ? "log_acacia_top":"log_acacia";
            case Game.BlockType.StoneBrickWhiteWall0: return faceIndex == 0 ? "white_stone" : "stonebrick_mossy/stonebrick_3";
            case Game.BlockType.StoneBrickWhiteWall1: return faceIndex == 1 ? "white_stone" : "stonebrick_mossy/stonebrick_3";
            case Game.BlockType.StoneBrickWhiteWall4: return faceIndex == 4 ? "white_stone" : "stonebrick_mossy/stonebrick_3";
            case Game.BlockType.StoneBrickWhiteWall5: return faceIndex == 5 ? "white_stone" : "stonebrick_mossy/stonebrick_3";//red_brick
            case Game.BlockType.Leaves      : return "leaves_acacia";
            case Game.BlockType.WoodFloor : return "wood_floor";
            case Game.BlockType.RedSand     : return "red_sand";
            case Game.BlockType.WoolColoredBlack: return "wool_colored_black";
            case Game.BlockType.StoneSlab   : return faceIndex == 2 || faceIndex == 3 ? "stone_slab_top" : "stone_slab_side";
            case Game.BlockType.HardenedClayStainedBlue: return "hardened_clay_stained_blue";
            case Game.BlockType.StairLeft: return "stonebrick_mossy/stonebrick_3";
            case Game.BlockType.StairRight: return "stonebrick_mossy/stonebrick_3";
            case Game.BlockType.StairBack: return "stonebrick_mossy/stonebrick_3";
            case Game.BlockType.StairFront: return "stonebrick_mossy/stonebrick_3";
            default: return "sand";
        }
    }
    public static Color GetColor(Game.BlockType block, int faceIndex)
    {
        switch (block) {
            case Game.BlockType.Leaves      :return Color.green;
            default: return Color.white;
        }
    }
}

public class TexturePacker
{
    Texture2D atlas = new Texture2D(1024,1024,TextureFormat.ARGB32,false);
    string[] names = new string[(int)Game.BlockType.Num * 6];
    Dictionary<string, Rect> packResult = new Dictionary<string, Rect>();
    public void AddTexture(Game.BlockType block, int faceIndex,string texName)
    {
        names[(int)block * 6 + faceIndex] = texName;
    }
    public Texture2D GetPackedTexture()
    {
        return atlas;
    }
    public void Pack()
    {
        Texture2D errImage = new Texture2D(1,1,TextureFormat.ARGB32,false);
        errImage.SetPixel(0, 0, new Color(1, 0, 1));
        errImage.Apply();

        //去重
        Dictionary<string, Texture2D> imageDict = new Dictionary<string, Texture2D>();
        for (int i = 0; i < names.Length; i++) {
            if (!imageDict.ContainsKey(names[i])) {
                Texture2D image = Resources.Load("Textures/blocks/" + names[i], typeof(Texture2D)) as Texture2D;
                if (image != null) {
                    imageDict[names[i]] = image;
                }
            }
        }

        //变成数组备用
        List<Texture2D> images = new List<Texture2D>();
        foreach(var item in imageDict){
            images.Add(item.Value);
        }
        
        //pack并生成坐标
        atlas = new Texture2D(1, 1);
        Rect[] rects = atlas.PackTextures(images.ToArray(),0);
        atlas.filterMode = FilterMode.Point;
        OpenGL.TextureParameter(atlas.GetNativeTexturePtr(), OpenGL.GL_TEXTURE_MAX_LEVEL, 6);

        //将结果放到字典中
        int index = 0;
        foreach (var item in imageDict) {
            packResult[item.Key] = rects[index];
            index++;
        }

    }
    public Rect GetRect(Game.BlockType block, int faceIndex)
    {
        string name = names[(int)block * 6 + faceIndex];
        return packResult[name];
    }
}
