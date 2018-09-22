using UnityEngine;
using System.Collections;

public class Timer : ScriptableObject {
    public float time { get { return Time.time; } }
    public float deltaTime { get { return Time.deltaTime; } }
}
