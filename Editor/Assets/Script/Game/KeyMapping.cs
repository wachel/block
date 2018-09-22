using UnityEngine;
using System.Collections;

public enum GameKeyCode
{
    Forward,
    Left,
    Right,
    Backward,
    Jump,
}

public static class KeyMapping
{
    public static bool IsKeyDown(GameKeyCode key)
    {
        switch (key) {
            case GameKeyCode.Forward: return Input.GetKey(KeyCode.W);
            case GameKeyCode.Left: return Input.GetKey(KeyCode.A);
            case GameKeyCode.Right: return Input.GetKey(KeyCode.D);
            case GameKeyCode.Backward: return Input.GetKey(KeyCode.S);
            case GameKeyCode.Jump: return Input.GetKey(KeyCode.Space);
            default: return false;
        }
    }
}
