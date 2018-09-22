Shader "Custom/Perlin2D" {
	Properties {
		 _Seed("Seed",Int) = 0
		 _Frequency("Frequency",Float) = 1
		 _OffsetX("Offset X",Float) = 0
		 _OffsetY("Offset Y",Float) = 0
		 _Factor("Factor",Float) = 1
	}
	SubShader {
        Pass {
			Tags { "QUEUE"="Transparent" "IGNOREPROJECTOR"="true" "RenderType"="Transparent" }
			ZWrite Off
			Cull Off
			Blend One One
            CGPROGRAM

			#pragma target 3.0
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
			#include "noiseSimplex.cginc"

            struct v2f {
				float4 pos : SV_POSITION;
                float2 uv : TEXCOORD0;
            };

            v2f vert (appdata_base v)
            {
                v2f o;
				o.pos = mul (UNITY_MATRIX_MVP, v.vertex);
				o.uv = v.texcoord;
                return o;
            }

			int _Seed;
			float _OffsetX,_OffsetY;
			float _Frequency;
			float _Factor;
            float4 frag (v2f i) : SV_Target
            {
				float seed = fmod(_Seed,16807) * 1.234567;
				float2 offset = float2(_OffsetX,_OffsetY);
				float v = snoise(float2(i.uv.x + seed,i.uv.y) * _Frequency + offset) * 0.5 + 0.5;
                return float4(v,v,v,1) * _Factor ;
            }
            ENDCG
        }
	} 
	FallBack "Diffuse"
}
