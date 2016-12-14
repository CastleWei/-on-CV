#include <opencv.hpp>

using namespace cv;

//���� ������������һ������230�㣬���������ҵ���һ��С��230�㣬�Ӵ˷�Χ������������150�㣬֮�����ķ�
void PeanutDivide(uchar *p, int w, int h)
{
	int base = 0;		//�е�ַ
	for (int i = 0; i < h; i++)			//������
	{
		int middle = 0;
		int left = 0, right = 0, j = 0;
		for (j = 0; j < w; j++)			//������������һ������230��
		{
			if (p[base + j] >= 200)
			{
				left = j;
				break;
			}
		}
		for (j; j < w; j++)		//���������ҵ���һ��С��150��
		{
			if (p[base + j] < 150)
			{
				right = j - 1;
				break;
			}
		}
		for (j = left - 1; j>0; j--)		//������������˵�
		{
			if (p[base + j] < 150)
			{
				left = j + 1;
				break;
			}
		}
		if (right != 0 || left != 0)
		{
			int sum = 0, sum1 = 0;
			for (j = left; j <= right; j++)		//���ķ�
			{
				sum += j*p[base + j];
				sum1 += p[base + j];
			}
			middle = ((double)sum + 0.5) / sum1;
		}
		for (j = 0; j < w; j++)			//Ĩ��
		{
			p[base + j] = 0;
		}
		p[base + middle] = middle == 0 ? 0 : 255;
		base += w;
	}
}

//���� OTSU
void OtsuDivide(uchar *p, int w, int h, int *offs)
{
	//��otsu������ȡȫ����ֵ��Ȼ�����ķ���ȡ�⵶����
	int arr[256] = { 0 };//�Ҷ�ֱ��ͼ
	int base = 0;		//������
	double sum = 0, ave = 0;
	for (int i = 0; i < h; i++)			//�Ҷ�ֱ��ͼͳ��,ȫ�־�ֵ����
	{
		for (int j = 0; j < w; j++)
		{
			arr[p[base + j]]++;
			sum += p[base + j];
		}
		base += w;
	}
	ave = sum / (w*h);		//ȫ�־�ֵ
	int threshold = 0;
	double max = 0;
	double ave1 = 0, ave2 = 0, sum1 = 0, sum2 = 0, count1 = 0, count2 = 0;

	for (int i = 150; i < 256; i++)		//�жϺ�ʱȡ�������
	{
		ave1 = 0, ave2 = 0, sum1 = 0, sum2 = 0, count1 = 0, count2 = 0;
		for (int j = 0; j < i; j++)
		{
			sum1 += arr[j] * j;
			count1 += arr[j];
		}
		for (int j = i; j < 256; j++)
		{
			sum2 += arr[j] * j;
			count2 += arr[j];
		}
		ave1 = sum1 / count1;
		ave2 = sum2 / count2;
		if (max <= ((ave - ave1)*(ave2 - ave)))
		{
			max = ((ave - ave1)*(ave2 - ave));
			threshold = i;
		}
	}
	printf("%d,", threshold);
// 	if (threshold == 0) waitKey(0);
	base = 0;
	// 		for (int i = 0; i < h; i++)			//��ֵ��
	// 		{
	// 			for (int j = 0; j < w; j++)
	// 			{
	// 				if (p[base+j]>=threshold)
	// 				{
	// 					p[base + j] = 255;
	// 				}
	// 				else
	// 				{
	// 					p[base + j] = 0;
	// 				}
	// 			}
	// 			base += w;
	// 		}

	for (int i = 0; i < h; i++)			//ÿ�����ķ�
	{
		int sum3 = 0, count3 = 0;
		for (int j = 0; j < w; j++)
		{
			if (p[base + j] >= threshold)
			{
				sum3 += j*p[base + j];
				count3 += p[base + j];
			}
			p[base + j] = 0;
		}
		if (sum3 != 0)
		{
			int oppo = (sum3 + 0.5) / count3;
			p[base + oppo] = 255;
		}
		base += w;

	}


}

//����Ӧ��ֵ��ͼ��ָ��ֵ���µ��ú�
void adaptive(Mat img){
	int h = img.rows;
	int w = img.cols;
	int step = img.step1();
	uchar *data = img.data;

	uchar th_h = 100; //�⵶����ֵ
	uchar th_d = 50; //��ֵ������ delta

	int base = 0; //ĳ�л�ַ
	for (int i = 0; i < h; i++){

		int iMax = 0; //ÿ�����ֵ������ֵ����ʼֵΪ0
		uchar max = data[base + 0]; //��һ�����ص�ֵ

		//Ѱ��ÿ�����ֵ
		for (int j = 1; j < w; j++){
			if (data[base + j]>max){
				iMax = j;
				max = data[base + j];
			}
		}

		if (max < th_h){
			//δ�ﵽ��ֵ����Ϊ����û�й⵶
			for (int j = 0; j < w; j++) data[base + j] = 0;
		}
		else{
			int th_l = max - th_d; //�⵶����ֵ
			
			//�����ұ�Ե
			int rightEdge = iMax + 1;
			while (data[base + rightEdge] > th_l) rightEdge++;

			int leftEdge = iMax - 1;
			while (data[base + leftEdge] > th_l) leftEdge--;

			//�ú���������
			for (int j = 0; j <= leftEdge; j++) data[base + j] = 0;
			for (int j = rightEdge; j < w; j++) data[base + j] = 0;
		}

		base += step; //������һ��
	}
}

//���ģ����ģ������õ�ƫ��������
void centroid(Mat img, int *offs){
	int h = img.rows;
	int w = img.cols;
	int step = img.step1();
	uchar *data = img.data;

	int base = 0; //ĳ�л�ַ
	for (int i = 0; i < h; i++){
		//���ӣ���ĸ
		int sum1 = 0, sum2 = 0;
		//���
		for (int j = 0; j < w; j++){
			uchar val = data[base + j];
			sum1 += val*j;
			sum2 += val;
		}

		// **һ��Ҫ�ǵ��жϷ�ĸΪ����������**
		int center = sum2 == 0 ? 0 : (int)((float)sum1 / sum2 + 0.5f); //����
		offs[i] = center;

		base += step; //������һ��
	}

}

//���ֵ��
void laserMax(Mat img, int *offs) {
	int h = img.rows;
	int w = img.cols;
	int step = img.step1();
	uchar *data = img.data;

	int base = 0; //ĳ�л�ַ
	for (int i = 0; i < h; i++){

		int iMax = 0; //ÿ�����ֵ������ֵ����ʼֵΪ0
		uchar max = data[base + 0]; //��һ�����ص�ֵ

		//Ѱ��ÿ�����ֵ
		for (int j = 1; j < w; j++){
			if (data[base + j]>max){
				iMax = j;
				max = data[base + j];
			}
		}
		//���ȴ�����ֵ����Ϊ�ǹ⵶
		offs[i] = max > 100 ? iMax : 0;

		base += step;
	}
}

//�е㷨
void midPoint(Mat img, uchar T, int *offs){
	int h = img.rows;
	int w = img.cols;
	int step = img.step1();
	uchar *data = img.data;

	//uchar T = 180; //��ֵ

	int base = 0; //ĳ�л�ַ
	for (int i = 0; i < h; i++){
		//������
		int left = 0;
		while (data[base + left] < T && left < w) left++;

		//û�ҵ�������ֵ�ĵ㣬����û�й⵶
		if (left == w) {
			offs[i] = 0;
		}
		else{
			//�½���
			int right = w - 1;
			while (data[base + right] < T && right > 0) right--;
			offs[i] = (left + right) >> 1; //�е㣬���Զ�
		}
		base += step;
	}
}

//����ȡOTSU����ֵ�����ú�
void otsuByRow(Mat img){
	int h = img.rows;
	int w = img.cols;
	int step = img.step1();
	uchar *data = img.data;

	int base = 0; //ĳ�л�ַ
	for (int i = 0; i < h; i++){
		int hist[256] = { 0 }; //ֱ��ͼhistogram
		//��һ�е�ֱ��ͼ
		for (int j = 0; j < w; j++){
			uchar gray = data[base + j];
			hist[gray]++;
		}

		uchar bestT = 0; //��󷽲��Ӧ��ֵ
		float maxG = 0; //��󷽲�
		int valSum0 = 0, valSum1 = 0; //�Ҷ�ֵ�ܺ�
		for (int val = 0; val < 256; val++)
			valSum1 += hist[val] * val;
		int N0 = 0, N1 = w, N = w; //��ֵ �������ҡ����� �����ظ�������ʼֵ��T=0ʱ

		for (uchar T = 0; T < 255; T++){
			int delta = hist[T];
			N0 += delta;
			N1 -= delta;
			valSum0 += delta * T;
			valSum1 -= delta * T;
			//��ƽ���Ҷ�
			float u0 = valSum0 / (float)N0;
			float u1 = valSum1 / (float)N1;
			//�µ�������ռ����
			float w0 = N0 / (float)N;
			float w1 = N1 / (float)N;
			//�·���
			float G = w0 * w1 * (u0 - u1) * (u0 - u1);
			//��ֵ���̫С�����
			if (T < 50) continue;
			if (G >= maxG){
				bestT = T;
				maxG = G;
			}
		}
		//if (bestT == 50){
		//	printf("%d", bestT);
		//	bestT = 255;
		//}
		//�����Ҷȣ����̫С��˵��û�й⵶
		int max = 255;
		while (hist[max] == 0) max--;
		if (max < 80) bestT = 255;

		//��������С����ֵ�Ĳ���ȥ��
		for (int j = 0; j < w; j++){
			if (data[base + j] < bestT)
				data[base + j] = 0;
		}

		//**************************
		base += step;
	}

}

//OTSU�����أ�ȡ������ֵ��Gs��������ֵ��Ӧ�ķ�������飬�������Ҫ������nullptr
uchar otsu(Mat img, float Gs[] = nullptr){
	int h = img.rows;
	int w = img.cols;
	int step = img.step1();
	uchar *data = img.data;

	int hist[256] = { 0 }; //ֱ��ͼhistogram
	//��ֱ��ͼ
	int base = 0;
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			uchar gray = data[base + j];
			hist[gray]++;
		}
		base += step;
	}

	uchar bestT = 0; //��󷽲��Ӧ��ֵ
	float maxG = 0; //��󷽲�
	int valSum0 = 0, valSum1 = 0; //�Ҷ�ֵ�ܺ�
	for (int val = 0; val < 256; val++)
		valSum1 += hist[val] * val;
	int N0 = 0, N1 = h*w, N = h*w; //��ֵ �������ҡ����� �����ظ�������ʼֵ��T=0ʱ

	for (uchar T = 0; T < 255; T++){
		int delta = hist[T];
		N0 += delta;
		N1 -= delta;
		valSum0 += delta * T;
		valSum1 -= delta * T;
		//��ƽ���Ҷ�
		float u0 = valSum0 / (float)N0;
		float u1 = valSum1 / (float)N1;
		//�µ�������ռ����
		float w0 = N0 / (float)N;
		float w1 = N1 / (float)N;
		//�·���
		float G = w0 * w1 * (u0 - u1) * (u0 - u1);
		if (Gs != nullptr) Gs[T] = G; //�м�����
		//��ֵ���̫С�����
		if (T < 50) continue;
		if (G > maxG){
			bestT = T;
			maxG = G;
		}
	}

	return bestT;
}

//ʹ����ֵ����ͼ��ָ������ֵ���ú�
void applyThreshold(Mat img, uchar T){
	int h = img.rows;
	int w = img.cols;
	int step = img.step1();
	uchar *data = img.data;

	int base = 0;
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			if (data[base + j] < T) data[base + j] = 0;
		}
		base += step;
	}
}

//������ͼ
Mat drawGraph(const float vals[], int count, int height, float max){
	Mat img(height + 1, count, CV_8UC1, Scalar(0));
	int h = img.rows;
	int w = img.cols;
	int step = img.step1();
	uchar *data = img.data;

	for (int x = 0; x < count; x++){
		int y = (int)(vals[x] / max * height + 0.5f); //������������������
		if (y > height) y = height; //Խ����
		int base = height * step + x;
		for (y; y > 0; y--){
			data[base] = 255;
			base -= step;
		}
	}

	return img;
}

//�ѹ⵶����ú��߻���ԭͼ��
void mix(Mat &src, int *offs){
	int h = src.rows;
	int w = src.cols;
	int step = src.step1();
	uchar *data = src.data;

	int base = 0; //ĳ�л�ַ
	for (int i = 0; i < h; i++){
		if (offs[i]>0){
			//���е�i�����ص��׵�ַ
			uchar *bgr = data + base + offs[i] * 3;
			//��Ϊ��ɫ
			bgr[0] = 0;
			bgr[1] = 0;
			bgr[2] = 255;
		}
		base += step;
	}
}

void EdgeDivide(uchar *p, int w, int h, int *offs)
//��Ե��⣬�ҳ�230���ϵĵ㣬�����߼����׵����仯�ĵ�
{
	//QElapsedTimer sec;
	//sec.start();

	int base = 0;
	for (int i = 0; i < h; i++)
	{
		// 		int *diff = new int[w];		//�ݶ�����
		// 		for (int j = 0; j < w - 1; j++)
		// 		{
		// 			diff[j] = p[base + j + 1] - p[base + j];
		// 		}
		// 		diff[w - 1] = 0;
		int left = 0;
		int right = w - 1;
		int j = 0;
		for (j = 0; j<w; j++)			//����ȡ230���ϵĵ�Ϊ�⵶���ĵ�
		{
			if (p[base + j]>150)
			{
				left = j - 1;
				break;
			}
		}
		for (j; j < w; j++)
		{
			if (p[base + j] < 150)
			{
				right = j;
				break;
			}
		}
		for (j = left; j > 1; j--)				//������Ѱ���ݶȽϴ�ĵ�
		{
			if (p[base + j + 1] + p[base + j - 1] - 2 * p[base + j] >= 0)
			{
				left = j;
				break;
			}
		}
		for (j = right; j < w - 1; j++)
		{
			if (p[base + j + 1] + p[base + j - 1] - 2 * p[base + j] <= 0)
			{
				right = j;
				break;
			}
		}
		if (left != 0 || right != w - 1)			//�й⵶�����ķ�
		{
			int count = 0;
			int sum = 0;
			int middle = 0;
			for (j = left; j <= right; j++)
			{
				sum += j*p[base + j];
				count += p[base + j];
			}
			middle = (sum + 0.5) / count;
			//for (j = 0; j < w; j++)
			//{
			//	p[base + j] = 0;
			//}
			//p[base + middle] = 255;

			//����ƫ��������
			offs[i] = middle;
		}
		else
		{
			offs[i] = 0;

			//δ�ҵ���Ĩ��
			//for (j = 0; j < w; j++)
			//{
			//	p[base + j] = 0;
			//}
		}
		// 		for (j = 0; j < w;j++)				//��ʾ��ȡ����
		// 		{
		// 			p[base + j] =0;
		// 		}
		// 		if (left!=0||right!=w-1)
		// 		{
		// 			for (j = left; j <= right;j++)
		// 			{
		// 				p[base + j] = 255;
		// 			}
		// 		}
		base += w;
	}
	//qDebug("%d", sec.elapsed());
}


void _0main(){
	Mat src; //ԭͼ��**��ɫΪBGR**
	Mat img; //�м���������**��ɫΪ�Ҷ�**
	Mat dst; //���ź���ʾ���
	Mat mat;
	Mat otsuImg; //otsu�м�ֵ
	float otsuVals[256] = { 0 };
	int *offs; //ƫ�����������
// 	VideoCapture cap("video.avi");
// 	cap >> src;
	src = imread("wgs.bmp");
	offs = new int[src.rows];

	while (1){
// 		bool ok = cap.read(src);
// 		if (!ok) break; //��Ƶ������˳�����������
		cvtColor(src, img, COLOR_BGR2GRAY); //����Ҷ�ͼ�����ڼ���

		//Ԥ���� �˲�
		//blur(img, img, Size(5, 5));
		//GaussianBlur(img, img, Size(5, 5), 0, 0);
		//medianBlur(img, img, 5);
		//bilateralFilter(mat, img, 4, 8, 2);
		//resize(mat, mat, Size(), 2, 2, INTER_NEAREST);
		//imshow("mat", mat);

		//ͼ��ָ�
		//adaptive(img);
		//otsuByRow(img);

		//uchar T = otsu(img, otsuVals);
		//applyThreshold(img, T);
		//resize(img, otsuImg, Size(), 2, 2, INTER_NEAREST);
		//imshow("test", img);

		//��OTSU��ֵ����ͼ
		//int maxT = 0;
		//for (int i = 0; i < 256; i++)
		//if (otsuVals[i] > otsuVals[maxT])
		//	maxT = i;
		//Mat mat = drawGraph(otsuVals, 256, 256, otsuVals[maxT]);
		//resize(mat, otsuImg, Size(), 2, 2, INTER_NEAREST);
		//imshow("test otsu", otsuImg);

		//Ӧ�ù⵶��ȡ�㷨
		//laserMax(img, offs);
		//midPoint(img, 200, offs);
		//centroid(img, offs);
		//PeanutDivide(src.data, src.cols, src.rows);
		//OtsuDivide(img.data, img.cols, img.rows, offs);
		EdgeDivide(img.data, img.cols, img.rows, offs);

		//���ƹ⵶����ʾ
		mix(src, offs);
		resize(src, dst, Size(), 2, 2, INTER_NEAREST);
		imshow("mixed img", dst);

		//Ĭ��5֡������������٣���esc==27�˳�
		if (waitKey(0) == 27) break;
	}
	

	//waitKey(0);
	if (offs != nullptr)
		delete offs;
}

