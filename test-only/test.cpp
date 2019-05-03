#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <ctime>
#include <fstream>

#ifndef MIN
#define MIN(A,B)    (((A) < (B)) ? (A) : (B))
#endif

#ifndef MAX
#define MAX(A,B)    (((A) > (B)) ? (A) : (B))
#endif

using namespace std;

typedef struct _Sample
{
    double *data;
    double *label;

    int sample_w;
    int sample_h;
    int sample_count;
} Sample;

typedef struct _Kernel
{
    double *W; 
    double *dW; //delta W
} Kernel;

typedef struct _Map
{
    double *data;
    double *error;
    double  b; // bias
    double  db; //delta b
} Map;


typedef struct _Layer
{
    int map_w;
    int map_h;
    int map_count;
    Map *map;

    int kernel_w;
    int kernel_h;
    int kernel_count;
    Kernel *kernel;

    double *map_common;
} Layer;


const int batch_size = 10;
const int classes_count = 10;
const int width  = 32;
const int height = 32;
const int train_sample_count = 60000;
const int test_sample_count  = 10000;

Layer input_layer, output_layer;
Layer c1_conv_layer, c3_conv_layer, c5_conv_layer;
Layer s2_pooling_layer, s4_pooling_layer;

int translateEndian_32(int i){
    return ((i & 0x000000FF) << 24 | (i & 0x0000FF00) << 8 | (i & 0x00FF0000) >> 8 | (i & 0xFF000000) >> 24);
}

void load_mnist_data(Sample * sample, const char * file_name){
    FILE *fp = NULL;
    fp = fopen(file_name, "rb");
    if(fp == NULL) {
        cout << "load mnist data failed." << endl;
        return;
    }
    int magic_number = 0;
    int sample_number = 0;
    int n_rows = 0, n_cols = 0;
    fread((int *)&magic_number, sizeof(magic_number), 1, fp);

    magic_number = translateEndian_32(magic_number);
//    cout << "magic number = " << magic_number << endl;

    fread((int *)&sample_number, sizeof(sample_number), 1, fp);
    sample_number = translateEndian_32(sample_number);
//    cout << "sample number = " << sample_number << endl;

    fread((int *)&n_rows, sizeof(n_rows), 1, fp);
    n_rows = translateEndian_32(n_rows);
//    cout << "n_rows = " << n_rows << endl;

    fread((int *)&n_cols, sizeof(n_cols), 1, fp);
    n_cols = translateEndian_32(n_cols);
//    cout << "n_cols = " << n_cols << endl;

    int zero_padding = 2;
    int padded_matrix_size = (n_cols + 2 * zero_padding) * (n_rows + 2 * zero_padding);
    unsigned char temp;
    double normalize_max = 1, normalize_min = -1;
//    double normalize_max = 1.175, normalize_min = -0.1;

    //test
//    sample_number = 1;
    cout << "Sample number: " << sample_number  << " " << padded_matrix_size  << " " << sizeof(double) << endl;
    int mem = 0;
    for(int k = 0; k < sample_number; k++){
        sample[k].data = (double *)malloc(padded_matrix_size * sizeof(double));
        mem+=padded_matrix_size * sizeof(double);
        memset(sample[k].data, 0, padded_matrix_size * sizeof(double));
        for(int i = 0; i < n_rows; i++){
            for(int j = 0; j < n_cols; j++){
                fread(&temp, 1, 1, fp);
//                cout << i << "  "<< j << "---" << (double)temp/255 << endl;
                sample[k].data[(i + zero_padding) * width + j + zero_padding] = (double)temp/255 * (normalize_max - normalize_min) + normalize_min;
            }
        }
    }
    cout << "mem used for input : " << mem << endl;
    fclose(fp);
    fp = NULL;
}



void load_mnist_label(Sample * sample, const char * file_name){
    FILE *fp = NULL;
    fp = fopen(file_name, "rb");
    if(fp == NULL) {
        cout << "load mnist label failed." << endl;
        return;
    }
    int magic_number = 0;
    int sample_number = 0;
    fread((int *)&magic_number, sizeof(magic_number), 1, fp);

    magic_number = translateEndian_32(magic_number);
//    cout << "magic number = " << magic_number << endl;

    fread((int *)&sample_number, sizeof(sample_number), 1, fp);
    sample_number = translateEndian_32(sample_number);
//    cout << "sample number = " << sample_number << endl;

    unsigned char temp;
    //test
//    sample_number = 100;
    for(int k = 0; k < sample_number; k++){
//        sample[k].label = (double *)malloc(classes_count * sizeof(double));
//        memset(sample[k].label, 0, classes_count * sizeof(double));
//        fread(&temp, 1, 1, fp);
//        sample[k].label[(int)temp] = 1;
//        cout << "label == " << (int)temp << endl;

        sample[k].label = (double *)malloc(classes_count * sizeof(double));
        for (int i = 0; i < classes_count; i++) {
            sample[k].label[i] = -0.8;
        }

        fread((char*)&temp, sizeof(temp), 1, fp);
        sample[k].label[temp] = 0.8;
    }
    fclose(fp);
    fp = NULL;
}

double** inputWeights(string file_name, int map_count, int size, int* mem_used){
    int mem = 0;
	ifstream file(file_name.c_str(), ios::in);
	// file.open(file_name);

	double **kernel = (double **)malloc(map_count*sizeof(double*));
    mem+=map_count*sizeof(double*);
	string line;
	string waste_buffer;
	if(file.is_open()){

		for(int i = 0; i < map_count; i++){
			double* subkernel = (double *)malloc(size*sizeof(double));
            mem+=size*sizeof(double);
			for (int j = 0; j < size; j++){
				getline(file, line);
				subkernel[j] = strtod(line.c_str(), NULL);
			}
			kernel[i] = subkernel;
		}
	}
    *mem_used = *mem_used + mem;

	file.close();
	return kernel;
}

double *inputBias(string file_name, int map_count, int* mem_used){
    int mem = 0;
	ifstream file(file_name.c_str(), ios::in);
	// file.open(file_name);
	double *bias = (double*) malloc (map_count * sizeof(double));
    mem+=map_count * sizeof(double);

	string line;
	string waste_buffer;
	if(file.is_open()){

		for(int i = 0; i < map_count; i++){
				getline(file, line);
				bias[i] = strtod(line.c_str(), NULL);
		}
	}
	file.close();
    *mem_used = *mem_used + mem;
	return bias;
}

void init_layer(Layer *layer, int prevlayer_map_count, int map_count, 
    int kernel_w, int kernel_h, int map_w, int map_h, bool is_pooling, int fileFlag) {

    cout << "layer " << fileFlag << " :" << endl;
    int mem_used = 0;
    // cout << "prevlayer_map_count: " <<  prevlayer_map_count << endl;
    // cout << "map_count: " <<  map_count << endl;
    // cout << "kernel count: " << prevlayer_map_count * map_count << endl;
    // cout << "kernel size: " <<  kernel_w * kernel_h << endl;
    // cout << "map w/h: " << map_w << "/" << map_h << endl << endl;

    string wPath;
    string bPath;
    if (fileFlag == 2){
        wPath = "c1w.txt";
        bPath = "c1b.txt";
    } else if (fileFlag == 4){
        wPath = "c3w.txt";
        bPath = "c3b.txt";
    } else if (fileFlag == 6){
        wPath = "c5w.txt";
        bPath = "c5b.txt";
    } else if (fileFlag == 7){
        wPath = "ow.txt";
        bPath = "ob.txt";
    }


    int mem_size = 0;

    const double scale = 6.0;
    int fan_in = 0;
    int fan_out = 0;
    if (is_pooling) {
        fan_in  = 4;
        fan_out = 1;
    }
    else {
        fan_in = prevlayer_map_count * kernel_w * kernel_h;
        fan_out = map_count * kernel_w * kernel_h;
    }
    int denominator = fan_in + fan_out;
    double weight_base = (denominator != 0) ? sqrt(scale / (double)denominator) : 0.5;

    layer->kernel_count = prevlayer_map_count * map_count;
    layer->kernel_w = kernel_w;
    layer->kernel_h = kernel_h;
    layer->kernel = (Kernel *)malloc(layer->kernel_count * sizeof(Kernel));
    mem_used+=layer->kernel_count * sizeof(Kernel);
    mem_size = layer->kernel_w * layer->kernel_h * sizeof(double);
    double **vir_k = inputWeights(wPath, map_count*prevlayer_map_count, kernel_w*kernel_h, &mem_used);
    for (int i = 0; i < prevlayer_map_count; i++) {
        for (int j = 0; j < map_count; j++) {
            layer->kernel[i*map_count + j].W = vir_k[i*map_count + j];
        }
    }

    layer->map_count = map_count;
    layer->map_w = map_w;
    layer->map_h = map_h;
    layer->map = (Map *)malloc(layer->map_count * sizeof(Map));
    mem_used+=layer->map_count * sizeof(Map);
    mem_size = layer->map_w * layer->map_h * sizeof(double);
    double* vir_b = inputBias(bPath, map_count, &mem_used);
    for (int i = 0; i < layer->map_count; i++) {
        layer->map[i].b = fileFlag == 0 ? 0.0 : vir_b[i];
        layer->map[i].db = 0.0;
        layer->map[i].data = (double *)malloc(mem_size);
        layer->map[i].error = (double *)malloc(mem_size);
        mem_used+=mem_size*2;
        memset(layer->map[i].data, 0, mem_size);
        memset(layer->map[i].error, 0, mem_size);
    }
    layer->map_common = (double *)malloc(mem_size);
    mem_used+=mem_size*2;
    memset(layer->map_common, 0, mem_size);
    cout << "mem used: " << mem_used << endl << endl;
}


void reset_params(Layer *layer)
{
    int mem_size = layer->kernel_w * layer->kernel_h * sizeof(double);
    for (int i = 0; i < layer->kernel_count; i++) {
        memset(layer->kernel[i].dW, 0, mem_size);
    }

    for (int i = 0; i < layer->map_count; i++) {
        layer->map[i].db = 0.0;
    }
}

void reset_weights() {
    reset_params(&c1_conv_layer);   // C1
    reset_params(&s2_pooling_layer);// S2
    reset_params(&c3_conv_layer);   // C3
    reset_params(&s4_pooling_layer);// S4
    reset_params(&c5_conv_layer);   // C5
    reset_params(&output_layer);    // Out
}



struct activation_func {
    /* scale: -0.8 ~ 0.8 和label初始值对应 */
    inline static double tan_h(double val) {
        double ep = exp(val);
        double em = exp(-val);

        return (ep - em) / (ep + em);
    }

    inline static double atan_h(double val) {
        return 1/(val*val+1);
    }

    inline static double dtan_h(double val) {
        return 1.0 - val*val;
    }

    inline static double relu(double val) {
        return val > 0.0 ? val : 0.0;
    }

    inline static double drelu(double val) {
        return val > 0.0 ? 1.0 : 0.0;
    }

    inline double sigmoid(double val) {
        return 1.0 / (1.0 + exp(-val));
    }

    inline double dsigmoid(double val) {
        return val * (1.0 - val);
    }
};



struct loss_func {
    inline static double mse(double y, double t) {
        return (y - t) * (y - t) / 2;
    }

    inline static double dmse(double y, double t) {
        return y - t;
    }
};

void convn_valid(double *in_data, int in_w, int in_h, double *kernel, int kernel_w, int kernel_h, double *out_data, int out_w, int out_h) {
    double sum = 0.0;
    for (int i = 0; i < out_h; i++) {
        for (int j = 0; j < out_w; j++) {
            sum = 0.0;
            for (int n = 0; n < kernel_h; n++) {
                for (int m = 0; m < kernel_w; m++) {
                    sum += in_data[(i + n)*in_w + j + m] * kernel[n*kernel_w + m];
                }
            }
            out_data[i*out_w + j] += sum;
        }
    }
}


#define O true
#define X false

bool connection_table[6*16] = {
                O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
                O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
                O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
                X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
                X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
                X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
};
#undef O
#undef X


void conv_fprop(Layer *prev_layer, Layer *layer, bool *pconnection) {
    int index = 0;
    int size = layer->map_w * layer->map_h;

    for (int i = 0; i < layer->map_count; i++) {
        memset(layer->map_common, 0, size*sizeof(double));
        for (int j = 0; j < prev_layer->map_count; j++) {
            index = j*layer->map_count + i;
            if (pconnection != NULL && !pconnection[index]) {
                continue;
            }
            convn_valid(
                    prev_layer->map[j].data, prev_layer->map_w, prev_layer->map_h,
                    layer->kernel[index].W, layer->kernel_w, layer->kernel_h,
                    layer->map_common, layer->map_w, layer->map_h);
        }

        for (int k = 0; k < size; k++) {
            layer->map[i].data[k] = activation_func::tan_h(layer->map_common[k] + layer->map[i].b);
        }
    }
}


void max_pooling_fprop(Layer *prev_layer, Layer *layer) {
    int map_w = layer->map_w;
    int map_h = layer->map_h;
    int upmap_w = prev_layer->map_w;

    for (int k = 0; k < layer->map_count; k++) {
        for (int i = 0; i < map_h; i++) {
            for (int j = 0; j < map_w; j++) {
                double max_value = prev_layer->map[k].data[2*i*upmap_w + 2*j];
                for (int n = 2*i; n < 2*(i + 1); n++) {
                    for (int m = 2*j; m < 2*(j + 1); m++) {
                        max_value = MAX(max_value, prev_layer->map[k].data[n*upmap_w + m]);
                    }
                }
                layer->map[k].data[i*map_w + j] = activation_func::tan_h(max_value);
            }
        }
    }
}


void fully_connected_fprop(Layer *prev_layer, Layer *layer) {
    for (int i = 0; i < layer->map_count; i++) {
        double sum = 0.0;
        for (int j = 0; j < prev_layer->map_count; j++) {
            sum += prev_layer->map[j].data[0] * layer->kernel[j*layer->map_count + i].W[0];
        }
        sum += layer->map[i].b;
        layer->map[i].data[0] = activation_func::tan_h(sum);
    }
}


void forward_propagation(){

    // In-->C1
    // cout << "c1 fprop..." << endl;
    conv_fprop(&input_layer, &c1_conv_layer, NULL);

    // C1-->S2
    // cout << "s2 fprop..." << endl;
    max_pooling_fprop(&c1_conv_layer, &s2_pooling_layer);

    // S2-->C3
    // cout << "c3 fprop..." << endl;
    conv_fprop(&s2_pooling_layer, &c3_conv_layer, connection_table);

    // C3-->S4
    // cout << "s4 fprop..." << endl;
    max_pooling_fprop(&c3_conv_layer, &s4_pooling_layer);

    // S4-->C5
    // cout << "c5 fprop..." << endl;
    conv_fprop(&s4_pooling_layer, &c5_conv_layer, NULL);

    // cout << "finished!" << endl << endl;

    // C5-->Out
    fully_connected_fprop(&c5_conv_layer, &output_layer);
}

int find_index(Layer * layer){
    int index = 0;
    double max_val = *(layer->map[0].data);
    for (int i = 1; i < layer->map_count; i++) {
        if (*(layer->map[i].data) > max_val) {
            max_val = *(layer->map[i].data);
            index = i;
        }
    }

    return index;
}

int find_index(double *label) {
    int index = 0;
    double max_val = label[0];
    for (int i = 1; i < classes_count; i++) {
        if (label[i] > max_val) {
            max_val = label[i];
            index = i;
        }
    }

    return index;
}

void save_l_output(Layer* layer, string layer_name){
    int m_size = layer->map_w*layer->map_h;
    ofstream weight_file(layer_name.c_str());
    for (int i = 0; i < layer->map_count; i++){
        for (int k = 0; k < m_size; k++){
            weight_file << layer->map[i].data[k] << endl;
        }
    }
    weight_file.close();
}

void save_output(){
    save_l_output(&input_layer, "io.txt");
    save_l_output(&c1_conv_layer, "c1o.txt");
    save_l_output(&s2_pooling_layer, "s2o.txt");
    save_l_output(&c3_conv_layer, "c3o.txt");
    save_l_output(&s4_pooling_layer, "s4o.txt");
    save_l_output(&c5_conv_layer, "c5o.txt");
    save_l_output(&output_layer, "oo.txt");
}


void predict(Sample * test_sample) {
    int num_success = 0, predict = 0, actual = 0;
    int data_mem_size = test_sample->sample_h * test_sample->sample_w * sizeof(double);
    int *confusion_matrix = (int *)malloc(classes_count * classes_count * sizeof(int));
    memset(confusion_matrix, 0, classes_count * classes_count * sizeof(int));

    for (int i = 0; i < test_sample->sample_count; i++) {
        memcpy(input_layer.map[0].data, test_sample[i].data, data_mem_size);
        // cout << i << endl;
        forward_propagation();
        if (i == 0) save_output();

        predict = find_index(&output_layer);
        actual = find_index(test_sample[i].label);
        if (predict == actual) {
            num_success++;
        }

        confusion_matrix[predict*classes_count + actual]++;
    }

    printf("accuracy: %d/%d\n", num_success, test_sample->sample_count);
    printf("\n   *  ");
    for (int i = 0; i < classes_count; i++)
    {
        printf("%4d  ", i);
    }

    printf("\n");
    for (int i = 0; i < classes_count; i++)
    {
        printf("%4d  ", i);
        for (int j = 0; j < classes_count; j++)
        {
            printf("%4d  ", confusion_matrix[i*classes_count + j]);
        }
        printf("\n");
    }
    printf("\n");

    free(confusion_matrix);
    confusion_matrix = NULL;
}

void release_layer(Layer *layer)
{
    for (int i = 0; i < layer->kernel_count; i++)
    {
        free(layer->kernel[i].W);
        free(layer->kernel[i].dW);
        layer->kernel[i].W = NULL;
        layer->kernel[i].dW = NULL;
    }
    free(layer->kernel);
    layer->kernel = NULL;

    for (int i = 0; i < layer->map_count; i++)
    {
        free(layer->map[i].data);
        free(layer->map[i].error);
        layer->map[i].data = NULL;
        layer->map[i].error = NULL;
    }
    free(layer->map_common);
    layer->map_common = NULL;
    free(layer->map);
    layer->map = NULL;
}

int main() {
    int kernel_w = 0, kernel_h = 0;

    cout << "Sample: " << sizeof(Sample) << endl;
    cout << "Kernel: " << sizeof(Kernel) << endl;
    cout << "Map: " << sizeof(Map) << endl;
    cout << "Layer: " << sizeof(Layer) << endl;
    
    //加载测试数据集文件
    Sample * test_sample = (Sample *)malloc(test_sample_count * sizeof(Sample));
    memset(test_sample, 0, test_sample_count * sizeof(Sample));
    test_sample->sample_h = height;
    test_sample->sample_w = width;
    test_sample->sample_count = test_sample_count;

    const char* test_sample_path = "t10k-images.idx3-ubyte";
    const char* test_label_path = "t10k-labels.idx1-ubyte";

    load_mnist_data(test_sample, test_sample_path);
    load_mnist_label(test_sample, test_label_path);

    ofstream input_label("input_labels.txt");
    ofstream input_data("input_data.txt");

    for (int i = 0; i < 1000; i++){

        input_label << find_index(test_sample[i].label) << "," << endl;

        for (int k = 0; k < 32*32; k++){
            input_data << test_sample[i].data[k] << "," << endl;
        }
    }

    input_label.close();
    input_data.close();

    kernel_w = 0;
    kernel_h = 0;
    init_layer(&input_layer, 0, 1, kernel_w, kernel_h, width, height, false, 1);

    kernel_w = 5;
    kernel_h = 5;
    init_layer(&c1_conv_layer, 1, 6, kernel_w, kernel_h, input_layer.map_w - kernel_w + 1, input_layer.map_h - kernel_h + 1, false, 2);

    kernel_w = 1;
    kernel_h = 1;
    init_layer(&s2_pooling_layer, 1, 6, kernel_w, kernel_h, c1_conv_layer.map_w / 2, c1_conv_layer.map_h / 2, true, 3);

    kernel_w = 5;
    kernel_h = 5;
    init_layer(&c3_conv_layer, 6, 16, kernel_w, kernel_h, s2_pooling_layer.map_w - kernel_w + 1, s2_pooling_layer.map_h - kernel_h + 1, false, 4);

    kernel_w = 1;
    kernel_h = 1;
    init_layer(&s4_pooling_layer, 1, 16, kernel_w, kernel_h, c3_conv_layer.map_w / 2, c3_conv_layer.map_h / 2, true, 5);

    kernel_w = 5;
    kernel_h = 5;
    init_layer(&c5_conv_layer, 16, 120, kernel_w, kernel_h, s4_pooling_layer.map_w - kernel_w + 1, s4_pooling_layer.map_h - kernel_h + 1, false, 6);

    kernel_w = 1;
    kernel_h = 1;
    init_layer(&output_layer, 120, 10, kernel_w, kernel_h, 1, 1, false, 7);


    clock_t start_time = clock();
    predict(test_sample);
    cout << "time used: " << (double)(clock()-start_time) / CLOCKS_PER_SEC << "s" << endl;


    // for (int i = 0; i < test_sample_count; i++) {
    //     free(test_sample[i].data);
    //     free(test_sample[i].label);
    //     test_sample[i].data = NULL;
    //     test_sample[i].label = NULL;
    // }
    // free(test_sample);

    // release_layer(&input_layer);
    // release_layer(&c1_conv_layer);
    // release_layer(&c3_conv_layer);
    // release_layer(&c5_conv_layer);
    // release_layer(&s2_pooling_layer);
    // release_layer(&s4_pooling_layer);
    // release_layer(&output_layer);

    return 0;
}
