Shader "Custom/ViewVoxel2" {
	Properties {
		_MainTex ("Albedo (RGB)", 3D) = "white" {}
		_Cutoff ("Alpha cutoff", Range(0,1)) = 0.5
		_Bit ("Depth", float) = 0
		_Color("Color" , Color) = (1,1,1,1)
		_TexelSize("TexelSize", Vector) = (1,1,1,0)
	}
	SubShader {
		Tags { "RenderType"="AlphaTest" "Queue"="AlphaTest"}
		Cull Off
		LOD 200
		
		CGPROGRAM
		#pragma surface surf Unlit noforwardadd noambient alphatest:_Cutoff

		//#pragma target 2.0
		#include "RaytraceInc.cginc"

		sampler3D _MainTex;

		struct Input {
			float2 uv_MainTex;
		};

		//不计算光照的光照函数
		half4 LightingUnlit(SurfaceOutput s, half3 lightDir, half atten) {
			return half4(s.Albedo,s.Alpha);
		}

		float _Bit;
		float4 _Color;
		float4 _TexelSize;

		fixed4 frag(Input IN)
		{
			float d = floor(_Bit / 8);
			float b = frac(_Bit / 8) * 8;
			fixed4 v = tex3D (_MainTex, float3(IN.uv_MainTex.x, d * _TexelSize.y, IN.uv_MainTex.y));
			float f = frac(floor(v.a * 255 + 0.5) / pow(2,b+1));
			float l = floor(f * 2);
			return fixed4(_Color.xyz,l);
		}

		void surf (Input IN, inout SurfaceOutput o) {
			fixed4 col = frag(IN);
			o.Albedo = col.xyz;
			o.Alpha = col.a;
		}
		ENDCG
	}
	FallBack "Diffuse"
}
