//测试某一bit位是否为1
float testBit64(float4 v, float bit) {
	float4 rangeStart = float4(0, 16, 32, 48);
	float4 rangeEnd = float4(16, 32, 48, 64);
	float4 mask = (bit >= rangeStart) * (bit < rangeEnd);
	float4 bit4 = pow(2.0, (bit - rangeStart)) * mask;
	float4 f = frac(v / (bit4 * 2 + 0.00000001)) * 2; //求余
	float4 l = floor(f);	//整除
	return dot(l,1);
}

////测试某一bit位是否为1
//float testBit64(float4 v, float bit) {
//	float bitScale[16] = { 1.0,1.0 / 2,1.0 / 4,1.0 / 8,1.0 / 16,1.0 / 32,1.0 / 64,1.0 / 128,1.0 / 256,1.0 / 512,1.0 / 1024,1.0 / 2048,1.0 / 4096,1.0 / 8192,1.0 / 16384,1.0 / 32768 };
//	float4 mask[4] = { float4(1,0,0,0),float4(0,1,0,0),float4(0,0,1,0),float4(0,0,0,1) };
//	float4 bit4 = bitScale[frac(bit / 16) * 16].xxxx * mask[floor(bit / 16)];
//	float4 f = frac(v * bit4 * 0.5); //求余
//	float4 l = floor(f * 2);	//整除
//	return dot(l, 1);
//}

//测试某一bit位是否为1
float4 testBit64_4(float4 v, float bit) {
	float4 rangeStart = float4(0, 16, 32, 48);
	float4 rangeEnd = float4(16, 32, 48, 64);
	float4 mask = (bit >= rangeStart) * (bit < rangeEnd);
	float4 bit4 = pow(2, (bit - rangeStart)) * mask;
	float4 f = frac(v / (bit4 * 2 + 0.00000001)) * 2; //求余
	float4 l = floor(f);	//整除
	return l;
}