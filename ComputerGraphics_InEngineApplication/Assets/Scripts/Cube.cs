using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cube : MonoBehaviour
{
    // Red material for the cube
    public Material redMat = null;
    // Green material for the cube
    public Material greenMat = null;

    // Keeps track of if the colour (red) is its base colour or the changed colour (green)
    bool beenChanged = false;

    // Keeps track of the cubes current material
    private Material currentCubeMaterial = null;

    private void Start()
    {
        // Set the material to red when the app starts
        GetComponent<MeshRenderer>().material = redMat;
    }

    private void Update()
    {
        // Check the material that is currently on the Cube
        currentCubeMaterial = GetComponent<MeshRenderer>().material;
    }

    public void ChangeColour()
    {
        // If the cube is currently red
         if (currentCubeMaterial != greenMat && beenChanged == false)
         {
            // Change the colour of the cube to green
            GetComponent<MeshRenderer>().material = greenMat;
            beenChanged = true;
         }
         // If the cube is currently green
        else if (currentCubeMaterial != redMat && beenChanged == true)
        {
            // Change the colour of the cube to red
            GetComponent<MeshRenderer>().material = redMat;
            beenChanged = false;
        }
    }
}
