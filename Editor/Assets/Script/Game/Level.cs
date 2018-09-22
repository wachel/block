using UnityEngine;
using System.Collections;
using Block;


public class Level : MonoBehaviour 
{
    private static Level g_Level;
    public static Level Instance { get { return g_Level; } }

    public PlayerConfig playerConfig;
    public Timer timer;
    public RayCastManager scene;
    private MoveController moveController;
    private Avatar avatar;

    public void Awake()
    {
        g_Level = this;
        timer = new Timer();
        avatar = gameObject.AddComponent<Avatar>();
        avatar.config = new PlayerConfig();
        avatar.pos = new Vector3(5, 4, 3);
        moveController = gameObject.AddComponent<MoveController>();
        moveController.avatar = avatar;
        moveController.level = this;
    }
   
}
