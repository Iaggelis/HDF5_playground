#include <H5Cpp.h>
#include <H5DataSpace.h>
#include <H5Dpublic.h>
#include <H5Fpublic.h>
#include <H5Ppublic.h>
#include <H5Spublic.h>
#include <H5Tpublic.h>
#include <H5public.h>
#include <H5version.h>

#include <array>

constexpr std::size_t N = 30;

auto main() -> int {

  std::array<int, N> Buffer;
  for (std::size_t i = 0; i < Buffer.size(); ++i) {
    Buffer[i] = i;
  }

  // Dataspace with unlimited dimensions
  hsize_t Rank       = 1;
  hsize_t Dims[1]    = {N};
  hsize_t MaxDims[1] = {H5S_UNLIMITED};
  auto DataSpace     = H5Screate_simple(Rank, Dims, MaxDims);

  // Create new file
  auto File = H5Fcreate("test.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  // Modify dataset properties, enable chunking
  auto DCreationPropertyList = H5Pcreate(H5P_DATASET_CREATE);
  hsize_t ChunkDims[1]       = {5};
  auto status = H5Pset_chunk(DCreationPropertyList, Rank, ChunkDims);

  hid_t LinkCreationProtertyList = {};
  hid_t DAccessProtertyList      = {};
  auto DataSet = H5Dcreate(File, "set1", H5T_NATIVE_INT, DataSpace,
                           LinkCreationProtertyList, DCreationPropertyList,
                           DAccessProtertyList);

  // extend the dataset
  hsize_t Size[1] = {N};
  status          = H5Dextend(DataSet, Size);

  // Select a hyperslab
  auto FileSpace    = H5Dget_space(DataSet);
  hsize_t Offset[1] = {0};
  status = H5Sselect_hyperslab(FileSpace, H5S_SELECT_SET, Offset, nullptr, Dims,
                               nullptr);

  // write data to the hyperslab
  status = H5Dwrite(DataSet, H5T_NATIVE_INT, DataSpace, FileSpace, H5P_DEFAULT,
                    Buffer.data());

  // Actually extend the dataset now
  Dims[0] += N;
  Size[0] = Dims[0];
  status  = H5Dextend(DataSet, Size);

  // Select a hyperslab
  FileSpace        = H5Dget_space(DataSet);
  Offset[0]        = N;
  hsize_t Dims2[1] = {N};
  status = H5Sselect_hyperslab(FileSpace, H5S_SELECT_SET, Offset, nullptr,
                               Dims2, nullptr);
  // Define memory space
  DataSpace = H5Screate_simple(Rank, Dims2, nullptr);

  // Write the data to the hyperslab
  status = H5Dwrite(DataSet, H5T_NATIVE_INT, DataSpace, FileSpace, H5P_DEFAULT,
                    Buffer.data());

  status = H5Pclose(DCreationPropertyList);
  status = H5Dclose(DataSet);
  status = H5Sclose(DataSpace);
  status = H5Sclose(FileSpace);
  status = H5Fclose(File);

  return 0;
}
