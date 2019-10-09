Shader "Unlit/PostEffectOutline"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        Cull Back
        BlendOp add
        Blend One Zero
        Ztest Off
        ZWrite Off

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

            sampler2D _MainTex;
            float4 _MainTex_ST;
            sampler2D _CameraDepthTexture;
            float4 _CameraDepthTexture_ST;
            float4 _CameraDepthTexture_TexelSize;
            float _OutlineThreshold;
            float4 _OutlineColor;
            float _OutlineThick;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _CameraDepthTexture);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // 4sampling法
                // 対角線上のテクセルを用意
                half2 DiffUV = i.uv - half2(_CameraDepthTexture_TexelSize.x, _CameraDepthTexture_TexelSize.y) * _OutlineThick;

                // i.uvの近隣の4ピクセルをサンプリングする
                half2 col00=Linear01Depth(tex2D(_CameraDepthTexture,DiffUV).r);
                half2 col10=Linear01Depth(tex2D(_CameraDepthTexture,half2(i.uv.x,DiffUV.y)).r);
                half2 col01=Linear01Depth(tex2D(_CameraDepthTexture,half2(DiffUV.x,i.uv.y)).r);
                half2 col11=Linear01Depth(tex2D(_CameraDepthTexture,i.uv).r);
                
                half2 col0011=col00-col11;
                half2 col1001=col10-col01;

                // 2乗和
                half OutlineValue = col0011 * col0011 + col1001 * col1001;

                // default
                fixed4 col = _OutlineColor;

                // check
                if(0>OutlineValue-_OutlineThreshold) col = tex2D(_MainTex, i.uv);
                
                return col;
            }
            ENDCG
        }
    }
}
