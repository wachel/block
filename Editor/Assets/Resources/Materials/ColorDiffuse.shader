Shader "Custom/ColorDiffuse" {
	Properties {
		_MainTex ("Texture", 2D) = "white" {}
	}
	SubShader {
		Pass {
			Tags { "RenderType"="Opaque" }
            CGPROGRAM

            #pragma vertex vert
            #pragma fragment frag
			#pragma multi_compile_fog
            #include "UnityCG.cginc"

			struct appdata {
				float4 vertex : POSITION;
				float4 color : COLOR;
				float4 texcoord0 : TEXCOORD0;
				//float4 texcoord1 : TEXCOORD1;
				//float4 tangent : TANGENT;
			};

            struct v2f {
				float4 pos : SV_POSITION;
                fixed4 color : COLOR;
				fixed2 uv0:TEXCOORD0;
				UNITY_FOG_COORDS(1)
				//fixed2 uv1:TEXCOORD1;
				//fixed4 ao :TANGENT;
            };

            v2f vert (appdata v)
            {
                v2f o;
				o.pos = mul (UNITY_MATRIX_MVP, v.vertex);
				o.color = v.color;
				o.uv0 = v.texcoord0.xy;
				UNITY_TRANSFER_FOG(o,o.pos);
				//o.uv1 = v.texcoord1.xy;
				//o.ao = v.tangent;
                return o;
            }

			sampler2D _MainTex;

            fixed4 frag (v2f i) : SV_Target
            {
				fixed4 texColor = tex2D(_MainTex,i.uv0);
				fixed4 vetexColor = i.color;
				//float ao = lerp(lerp(i.ao.x,i.ao.y,i.uv1.y),lerp(i.ao.w,i.ao.z,i.uv1.y),i.uv1.x);
				fixed4 col = fixed4(texColor.xyz * vetexColor.xyz * vetexColor.a, 1);
				
				UNITY_APPLY_FOG(i.fogCoord, col);
				return col;
            }
            ENDCG
        }

	} 
	FallBack "Diffuse"
}
