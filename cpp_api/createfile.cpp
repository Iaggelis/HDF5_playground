#include <H5Cpp.h>
#include <H5DataSpace.h>

#include <H5Ppublic.h>
#include <H5Spublic.h>
#include <H5public.h>
#include <array>

constexpr std::size_t N = 30;

auto main() -> int {

  H5::H5File File("test.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  hsize_t Rank    = 1;
  hsize_t Dims[1] = {1};
  H5::DataSpace Dataspace(Rank, Dims);

  hsize_t ArrayDims[1] = {N};
  H5::ArrayType Datatype(H5::PredType::NATIVE_INT, Rank, ArrayDims);

  auto Dataset = File.createDataSet("test", Datatype, Dataspace);

  std::array<int, N> Buffer;
  for (std::size_t i = 0; i < Buffer.size(); ++i) {
    Buffer[i] = i;
  }
  Dataset.write(Buffer.data(), Datatype, Dataspace);

  return 0;
}
