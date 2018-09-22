using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;

public class OpenGL
{
    public const int GL_RED = 0x1903;
    public const int GL_GREEN = 0x1904;
    public const int GL_BLUE = 0x1905;
    public const int GL_ALPHA = 0x1906;
    public const int GL_RGB = 0x1907;
    public const int GL_RGBA = 0x1908;
    public const int GL_RGB4 = 0x804F;
    public const int GL_RGB5 = 0x8050;
    public const int GL_RGB8 = 0x8051;
    public const int GL_RGB10 = 0x8052;
    public const int GL_RGB12 = 0x8053;
    public const int GL_RGB16 = 0x8054;
    public const int GL_RGBA2 = 0x8055;
    public const int GL_RGBA4 = 0x8056;
    public const int GL_RGB5_A1 = 0x8057;
    public const int GL_RGBA8 = 0x8058;
    public const int GL_RGB10_A2 = 0x8059;
    public const int GL_RGBA12 = 0x805A;
    public const int GL_RGBA16 = 0x805B;

    public const int GL_BYTE = 0x1400;
    public const int GL_UNSIGNED_BYTE = 0x1401;
    public const int GL_SHORT = 0x1402;
    public const int GL_UNSIGNED_SHORT = 0x1403;
    public const int GL_INT = 0x1404;
    public const int GL_UNSIGNED_INT = 0x1405;
    public const int GL_FLOAT = 0x1406;

    public const int GL_TEXTURE_1D = 0x0DE0;
    public const int GL_TEXTURE_2D = 0x0DE1;
    public const int GL_TEXTURE_3D = 0x806F;

    public const int GL_TEXTURE_MAX_LEVEL = 0x813D;

    //[DllImport("opengl32")]static public extern void glReadPixels(int x, int y, int width, int height, int format, int type, IntPtr buffer);
    [DllImport("RenderingPlugin", CallingConvention = CallingConvention.Cdecl)]
    static public extern void UpdateVoxel(IntPtr tex, int xoffset, int yoffset, int zoffset, int width, int height, int depth, int format, int type, [MarshalAs(UnmanagedType.LPArray)]byte[] data, int length);
    [DllImport("RenderingPlugin")]
    public static extern IntPtr GetRenderEventFunc();
    [DllImport("RenderingPlugin")]
    public static extern IntPtr TextureParameter(IntPtr tex,int name,int param);
}
