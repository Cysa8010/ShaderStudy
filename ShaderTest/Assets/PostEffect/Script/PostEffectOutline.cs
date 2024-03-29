﻿using UnityEngine;
using UnityEngine.Rendering;

//[RequireComponent(typeof(Camera))]
public class PostEffectOutline : MonoBehaviour
{
    [SerializeField]
    private Shader _shader;
    [SerializeField]
    private float _outlineThreshold = 0.01f;
    [SerializeField]
    private Color _outlineColor = Color.white;
    [SerializeField]
    private float _outlineThick = 1.0f;

    private Material _material;

    private void Awake()
    {
        Initialize();
    }

    private void Update()
    {
#if UNITY_EDITOR
        SetMaterialProperties();
#endif
    }

    private void Initialize()
    {
        var camera = GetComponent<Camera>();
        camera.depthTextureMode |= DepthTextureMode.Depth;
        
        _material = new Material(_shader);
        SetMaterialProperties();

    }

    private void OnRenderImage(RenderTexture source, RenderTexture dest)
    {
        Graphics.Blit(source,dest,_material);
    }

    private void SetMaterialProperties()
    {
        if (_material != null)
        {
            _material.SetFloat("_OutlineThreshold", _outlineThreshold);
            _material.SetColor("_OutlineColor", _outlineColor);
            _material.SetFloat("_OutlineThick", _outlineThick);
        }
    }
}
