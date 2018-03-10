#include <stdio.h>
#include <unordered_map>

using std::unordered_map;

template<class T, class K>
void FWriteMap(std::unordered_map<T, K> & map, std::string file_name)
{
	FILE * fp = fopen(file_name.c_str(), "wb");
	size_t size = map.size();
	fwrite(&size, sizeof(size_t), 1, fp);
	for (auto m : map)
	{
		fwrite(&m.first, sizeof(T), 1, fp);
		fwrite(&m.second, sizeof(K), 1, fp);
	}
	fclose(fp);
}

template<class T, class K>
void FReadMap(std::unordered_map<T, K> & map, std::string file_name)
{
	FILE * fp = fopen(file_name.c_str(), "rb");

	size_t get_size = 0;
	T first = 0;
	K second = 0;
	fread(&get_size, sizeof(size_t), 1, fp);
	for (size_t n = 0; n < get_size; n++)
	{
		fread(&first, sizeof(T), 1, fp);
		fread(&second, sizeof(K), 1, fp);
		map[first] = second;
	}
	fclose(fp);
}

void main(void)
{
	unordered_map<int, float> map;
	unordered_map<int, float> map_in;

	map[10] = 4.f;
	map[20] = 5.f;

	FWriteMap(map, "test.bin");
	FReadMap(map_in, "test.bin");
	
	for (auto & m : map)
	{
		printf("%3d: %f\n", m.first, m.second);
	}
	printf("\n");
	for (auto & m : map_in)
	{
		printf("%3d: %f\n", m.first, m.second);
	}

	rewind(stdin);
	getchar();
}