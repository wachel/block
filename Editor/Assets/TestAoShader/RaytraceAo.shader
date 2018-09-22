Shader "Hidden/RaytraceAo"
{
	Properties
	{
		_MainTex ("Texture", 3D) = "white" {}
		_TexelSize("TexelSize", Vector) = (1,1,1,0)
		_PointTex("SurfacePoint",2D) = "black" {}
		_ConstValueTex("ConstValue",2D) = "black" {}
		_NormalIndex("NormalIndex",int) = 0
		_Normal("Normal",Vector) = (0,0,0,0)
		_StartPos("Start Pos",Vector) = (0,0,0,0)
	}
	SubShader
	{
		// No culling or depth
		Cull Off ZWrite Off ZTest Always
		Pass
		{
			CGPROGRAM
			#pragma target 3.0
			#pragma vertex vert
			#pragma fragment frag
			//#pragma enable_d3d11_debug_symbols
			#include "UnityCG.cginc"
			#include "RaytraceInc.cginc"

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

			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = mul(UNITY_MATRIX_MVP, v.vertex);
				o.uv = v.uv;
				return o;
			}
			
			sampler3D _MainTex;
			sampler2D _PointTex;
			sampler2D _ConstValueTex;
			half4 _ConstValueTex_TexelSize;
			float4 _TexelSize;
			int _NormalIndex;
			fixed4 _Normal;
			float4 _StartPos;

			////  1 out, 3 in...
			//#define HASHSCALE1 .1031
			//float hash13(vec3 p3)
			//{
			//	p3  = fract(p3 * HASHSCALE1);
			//	p3 += dot(p3, p3.yzx + 19.19);
			//	return fract((p3.x + p3.y) * p3.z);
			//}

			fixed4 frag (v2f i) : SV_Target
			{
				half4 startPos = tex2D(_PointTex, i.uv) + _StartPos;//位置信息
				fixed totalBrighness = 0;
				int raysNum = 22;
				int rayLength = 8;
				for (int d = 0; d < raysNum; d++) {
					fixed brightness = 0;
					fixed hitLength = 0;
					fixed curValue = 1;
					fixed4 dir = tex2D(_ConstValueTex, float2(d, _NormalIndex) * _ConstValueTex_TexelSize.xy);
					half4 pos = startPos;
					for (int i = 0;i < rayLength; i++) {
						pos += dir;

						float bitY = floor(pos.y);
						float y_i = floor(bitY / 8);
						float y_f = frac(bitY / 8) * 8;

						float4 voxel = tex3D(_MainTex, (float3(pos.x,y_i,pos.z)) * _TexelSize.xyz);

						float f = frac(floor(voxel.a * 255 + 0.5) / pow(2,y_f + 1));
						float v = floor(f * 2);

						curValue *= (1 - v);

						hitLength += curValue;
					}
					brightness = hitLength / rayLength;
					//brightness = max(brightness,0.1);
					totalBrighness += brightness;
				}
				return (totalBrighness / raysNum).xxxx;
			}
			ENDCG
		}
	}
}
