Shader "Unlit/DepthTexture"
{
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        Cull Back
        BlendOp add
        Blend One Zero
        Ztest Off
        Zwrite Off

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            
            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _CameraDepthTexture;
            float4 _CameraDepthTexture_ST;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _CameraDepthTexture);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                fixed depth = Linear01Depth(tex2D(_CameraDepthTexture, i.uv));
                
                return fixed4(depth,depth,depth,1);
            }
            ENDCG
        }
    }
}
