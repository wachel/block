using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

public class AppStart : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {
        CommandLineReader.parse(System.Environment.CommandLine);
        if (CommandLineReader.hasOption("editor")) {
            SceneManager.LoadScene("ModelEditor");
        }
        else {
            SceneManager.LoadScene("Game");
        }
    }

    // Update is called once per frame
    void Update()
    {

    }
}
