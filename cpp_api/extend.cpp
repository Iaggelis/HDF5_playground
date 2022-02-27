#include <H5Cpp.h>

#include <array>

constexpr std::size_t N = 30;

auto main() -> int {

  std::array<int, N> Buffer;
  for (std::size_t i = 0; i < Buffer.size(); ++i) {
    Buffer[i] = i;
  }

  H5::H5File File("test2.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  // Dataspace with unlimited dimensions
  hsize_t Rank         = 1;
  hsize_t ArrayDims[1] = {N};
  hsize_t MaxDims[1]   = {H5S_UNLIMITED};
  hsize_t Dims[1]      = {1};
  H5::DataSpace DataSpace(Rank, Dims, MaxDims);

  H5::ArrayType DataType(H5::PredType::NATIVE_INT, Rank, ArrayDims);

  H5::DSetCreatPropList DSetCreatPropList;
  hsize_t ChunkDims[1] = {1};
  DSetCreatPropList.setChunk(Rank, ChunkDims);
  auto DataSet =
      File.createDataSet("set", DataType, DataSpace, DSetCreatPropList);

  hsize_t Size[1] = {1};
  DataSet.extend(Size);

  auto FileSpace    = DataSet.getSpace();
  hsize_t Offset[1] = {0};
  FileSpace.selectHyperslab(H5S_SELECT_SET, Dims, Offset, nullptr, nullptr);

  // write data to the hyperslab
  DataSet.write(Buffer.data(), DataType, DataSpace, FileSpace);

  // Actually extend the dataset now
  Size[0] = 2;
  DataSet.extend(Size);
  FileSpace = DataSet.getSpace();
  Offset[0] = 1;
  FileSpace.selectHyperslab(H5S_SELECT_SET, Dims, Offset, nullptr, nullptr);

  DataSpace = H5::DataSpace(Rank, Dims);

  for (std::size_t i = 0; i < Buffer.size(); ++i) {
    Buffer[i] = (i << 1);
  }
  // write data to the hyperslab
  DataSet.write(Buffer.data(), DataType, DataSpace, FileSpace);
  return 0;
}
