using UnityEngine;
using System.Collections;

public class PlayerConfig
{
    public float walkSpeed = 6.0f;
    public float runSpeed = 12.0f;
    public float height = 3.2f;
}


public class Avatar : MonoBehaviour 
{
    public PlayerConfig config;
    public Vector3 pos;
    public Vector3 currentVelocity;
    public float yaw;
    public float pitch;
    public float moveSpeed = 0f;

    public void Awake()
    {

    }

    public Vector3 GetDir()
    {
        return Quaternion.AngleAxis(yaw, Vector3.up) * Vector3.forward;
    }

	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
