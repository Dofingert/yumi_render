//
// Created by Dofingert on 2022/11/10.
//
int cnt_leading_zero_4bit(unsigned char input_4bit) // lut4-3 = 2 * lut6
{
  switch (input_4bit)
  {
	case 0:
	  return 4;
	case 1:
	  return 3;
	case 2:
	case 3:
	  return 2;
	case 4:
	case 5:
	case 6:
	case 7:
	  return 1;
	default:
	  return 0;
  }
}

int cnt_leading_zero_8bit(unsigned char input_8bit) // 2 LUT6 + 4LUT6
{
  int result_l = cnt_leading_zero_4bit((input_8bit >> 4) & 0xf);
  int result_r = cnt_leading_zero_4bit(input_8bit & 0xf);
  if (result_l & 4)
  {
	return result_l + result_r;
  }
  return result_l;
}

int cnt_leading_zero_16bit(unsigned short input_16bit) // 12 LUT6 + 1 Mux + 1 Adder
{
  int result_l = cnt_leading_zero_8bit((input_16bit >> 8) & 0xff); // 4 bit
  int result_r = cnt_leading_zero_8bit(input_16bit & 0xff);        // 4 bit
  if (result_l & 8)
  {
	return result_l + result_r; // 5 bit adder
  }
  return result_l; // 5bit 2-1 mux 4layer, about
}

int make_binary_float(bool sign, unsigned char exp, int M)
{
  int data = M & 0x7FFFFF;
  data |= ((int)sign << 31);
  data |= ((int)exp << 23);
  return data;
}

float unsigned_short_to_float(unsigned short input)
{
  int cnt_leading_zero = cnt_leading_zero_16bit(input);
  int exp = 126 - cnt_leading_zero;
  if (cnt_leading_zero == 16)
  {
	exp = 0;
  }
  int m = (int)input << (cnt_leading_zero + 8);
  float ret;
  *(int*)&ret = make_binary_float(false,exp,m);
  return ret;
}