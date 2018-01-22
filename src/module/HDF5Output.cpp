#ifdef CRPROPA_HAVE_HDF5

#include "radiopropa/module/HDF5Output.h"
#include "radiopropa/Version.h"
#include "kiss/logger.h"

#include <hdf5.h>
#include <cstring>

const hsize_t RANK = 1;
const hsize_t BUFFER_SIZE = 1024 * 16;

namespace radiopropa {

// map variant types to H5T_NATIVE 
hid_t variantTypeToH5T_NATIVE(Variant::Type type) {
	if (type == Variant::TYPE_INT64)
		return H5T_NATIVE_INT64;
	else if(type == Variant::TYPE_BOOL)
		return H5T_NATIVE_HBOOL;
	else if(type == Variant::TYPE_CHAR)
		return H5T_NATIVE_CHAR;
	else if(type == Variant::TYPE_UCHAR)
		return H5T_NATIVE_UCHAR;
	else if(type == Variant::TYPE_INT16)
		return H5T_NATIVE_INT16;
	else if(type == Variant::TYPE_UINT16)
		return H5T_NATIVE_UINT16;
	else if(type == Variant::TYPE_INT32)
		return H5T_NATIVE_INT32;
	else if(type == Variant::TYPE_UINT32)
		return H5T_NATIVE_UINT32;
	else if(type == Variant::TYPE_INT64)
		return H5T_NATIVE_INT64;
	else if(type == Variant::TYPE_UINT64)
		return H5T_NATIVE_UINT64;
	else if(type == Variant::TYPE_FLOAT)
		return H5T_NATIVE_FLOAT;
	else if(type == Variant::TYPE_DOUBLE)
		return H5T_NATIVE_DOUBLE;
	else if(type == Variant::TYPE_STRING)
		return H5T_C_S1;
	else
	{
		KISS_LOG_ERROR << "variantTypeToH5T_NATIVE:: Type: " << Variant::getTypeName(type) << " unknown.";
		throw std::runtime_error("No matching HDF type for Variant type");
	}
}

HDF5Output::HDF5Output(const std::string& filename) :  Output(), filename(filename), file(-1), sid(-1), dset(-1), dataspace(-1) {
}

HDF5Output::HDF5Output(const std::string& filename, OutputType outputtype) :  Output(outputtype), filename(filename), file(-1), sid(-1), dset(-1), dataspace(-1) {
	outputtype = outputtype;
}

HDF5Output::~HDF5Output() {
	close();
}

herr_t HDF5Output::insertVersion() {
	hid_t 	strtype, attr_space, version_attr;
	hsize_t dims = 0;
	herr_t 	status;

	strtype = H5Tcopy(H5T_C_S1);
	status = H5Tset_size(strtype, 100);

	attr_space = H5Screate_simple(0, &dims, NULL);
	version_attr = H5Acreate2(dset, "Version", strtype, attr_space, H5P_DEFAULT, H5P_DEFAULT);
	status = H5Awrite(version_attr, strtype, g_GIT_DESC);
	status = H5Aclose(version_attr);
	status = H5Sclose(attr_space);

	return status;
}

void HDF5Output::open(const std::string& filename) {
	file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

	sid = H5Tcreate(H5T_COMPOUND, sizeof(OutputRow));
	if (fields.test(TrajectoryLengthColumn))
		H5Tinsert(sid, "D", HOFFSET(OutputRow, D), H5T_NATIVE_DOUBLE);
	if (fields.test(RedshiftColumn))
		H5Tinsert(sid, "z", HOFFSET(OutputRow, z), H5T_NATIVE_DOUBLE);
	if (fields.test(SerialNumberColumn))
		H5Tinsert(sid, "SN", HOFFSET(OutputRow, SN), H5T_NATIVE_UINT64);
	if (fields.test(CurrentIdColumn))
		H5Tinsert(sid, "ID", HOFFSET(OutputRow, ID), H5T_NATIVE_INT32);
	if (fields.test(CurrentEnergyColumn))
		H5Tinsert(sid, "E", HOFFSET(OutputRow, E), H5T_NATIVE_DOUBLE);
	if (fields.test(CurrentPositionColumn) && oneDimensional)
		H5Tinsert(sid, "X", HOFFSET(OutputRow, X), H5T_NATIVE_DOUBLE);
	if (fields.test(CurrentPositionColumn) && not oneDimensional) {
		H5Tinsert(sid, "X", HOFFSET(OutputRow, X), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "Y", HOFFSET(OutputRow, Y), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "Z", HOFFSET(OutputRow, Z), H5T_NATIVE_DOUBLE);
	}
	if (fields.test(CurrentDirectionColumn) && not oneDimensional) {
		H5Tinsert(sid, "Px", HOFFSET(OutputRow, Px), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "Py", HOFFSET(OutputRow, Py), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "Pz", HOFFSET(OutputRow, Pz), H5T_NATIVE_DOUBLE);
	}
	if (fields.test(SerialNumberColumn))
		H5Tinsert(sid, "SN0", HOFFSET(OutputRow, SN0), H5T_NATIVE_UINT64);
	if (fields.test(SourceIdColumn))
		H5Tinsert(sid, "ID0", HOFFSET(OutputRow, ID0), H5T_NATIVE_INT32);
	if (fields.test(SourceEnergyColumn))
		H5Tinsert(sid, "E0", HOFFSET(OutputRow, E0), H5T_NATIVE_DOUBLE);
	if (fields.test(SourcePositionColumn) && oneDimensional) 
		H5Tinsert(sid, "X0", HOFFSET(OutputRow, X0), H5T_NATIVE_DOUBLE);
	if (fields.test(SourcePositionColumn) && not oneDimensional){
		H5Tinsert(sid, "X0", HOFFSET(OutputRow, X0), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "Y0", HOFFSET(OutputRow, Y0), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "Z0", HOFFSET(OutputRow, Z0), H5T_NATIVE_DOUBLE);
	}
	if (fields.test(SourceDirectionColumn) && not oneDimensional) {
		H5Tinsert(sid, "P0x", HOFFSET(OutputRow, P0x), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "P0y", HOFFSET(OutputRow, P0y), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "P0z", HOFFSET(OutputRow, P0z), H5T_NATIVE_DOUBLE);
	}
	if (fields.test(SerialNumberColumn))
		H5Tinsert(sid, "SN1", HOFFSET(OutputRow, SN1), H5T_NATIVE_UINT64);
	if (fields.test(CreatedIdColumn))
		H5Tinsert(sid, "ID1", HOFFSET(OutputRow, ID1), H5T_NATIVE_INT32);
	if (fields.test(CreatedEnergyColumn))
		H5Tinsert(sid, "E1", HOFFSET(OutputRow, E1), H5T_NATIVE_DOUBLE);
	if (fields.test(CreatedPositionColumn) && oneDimensional)
		H5Tinsert(sid, "X1", HOFFSET(OutputRow, X1), H5T_NATIVE_DOUBLE);
	if (fields.test(CreatedPositionColumn) && not oneDimensional) {
		H5Tinsert(sid, "X1", HOFFSET(OutputRow, X1), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "Y1", HOFFSET(OutputRow, Y1), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "Z1", HOFFSET(OutputRow, Z1), H5T_NATIVE_DOUBLE);
	}
	if (fields.test(CreatedDirectionColumn) && not oneDimensional) {
		H5Tinsert(sid, "P1x", HOFFSET(OutputRow, P1x), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "P1y", HOFFSET(OutputRow, P1y), H5T_NATIVE_DOUBLE);
		H5Tinsert(sid, "P1z", HOFFSET(OutputRow, P1z), H5T_NATIVE_DOUBLE);
	}
	if (fields.test(WeightColumn))
		H5Tinsert(sid, "weight", HOFFSET(OutputRow, weight), H5T_NATIVE_DOUBLE);
	
	size_t pos = 0;
	for(std::vector<Output::Property>::const_iterator iter = properties.begin();
			iter != properties.end(); ++iter)
	{
			hid_t type = variantTypeToH5T_NATIVE((*iter).defaultValue.getType());
			if (type == H5T_C_S1)
			{ // set size of string field to size of default value!
				type = H5Tcopy(H5T_C_S1);
				H5Tset_size(type, (*iter).defaultValue.toString().size());
			}

			H5Tinsert(sid, (*iter).name.c_str(), HOFFSET(OutputRow, propertyBuffer) + pos, type);
		  pos += (*iter).defaultValue.getSize();
	}
	if (pos >= propertyBufferSize)
	{
		KISS_LOG_ERROR << "Using " << pos << " bytes for properties output. Maximum is " << propertyBufferSize << " bytes.";
		throw std::runtime_error("Size of property buffer exceeded");
	}

	// chunked prop
	hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
	H5Pset_layout(plist, H5D_CHUNKED);
	hsize_t chunk_dims[RANK] = {BUFFER_SIZE};
	H5Pset_chunk(plist, RANK, chunk_dims);
	H5Pset_deflate(plist, 5);

	hsize_t dims[RANK] = {0};
	hsize_t max_dims[RANK] = {H5S_UNLIMITED};
	dataspace = H5Screate_simple(RANK, dims, max_dims);

	dset = H5Dcreate2(file, outputName.c_str(), sid, dataspace, H5P_DEFAULT, plist, H5P_DEFAULT);
	insertVersion();	

	H5Pclose(plist);

	buffer.reserve(BUFFER_SIZE);
	time(&lastFlush);
}

void HDF5Output::close() {
	if (file >= 0) {
		flush();
		H5Dclose(dset);
		H5Tclose(sid);
		H5Sclose(dataspace);
		H5Fclose(file);
		file = -1;
	}
}

void HDF5Output::process(Candidate* candidate) const {
	#pragma omp critical
	{
	if (file == -1)
		// This is ugly, but necesary as otherwise the user has to manually open the
		// file before processing the first candidate 
		const_cast<HDF5Output*>(this)->open(filename);
	}

	OutputRow r;
	r.D = candidate->getTrajectoryLength() / lengthScale;
	r.z = candidate->getRedshift();

	r.SN = candidate->getSerialNumber();
	r.ID = candidate->current.getId();
	r.E = candidate->current.getEnergy() / energyScale;
	Vector3d v = candidate->current.getPosition() / lengthScale;
	r.X = v.x;
	r.Y = v.y;
	r.Z = v.z;
	v = candidate->current.getDirection();
	r.Px = v.x;
	r.Py = v.y;
	r.Pz = v.z;

	r.SN0 = candidate->getSourceSerialNumber();
	r.ID0 = candidate->source.getId();
	r.E0 = candidate->source.getEnergy() / energyScale;
	v = candidate->source.getPosition() / lengthScale;
	r.X0 = v.x;
	r.Y0 = v.y;
	r.Z0 = v.z;
	v = candidate->source.getDirection();
	r.P0x = v.x;
	r.P0y = v.y;
	r.P0z = v.z;

	r.SN1 = candidate->getCreatedSerialNumber();
	r.ID1 = candidate->created.getId();
	r.E1 = candidate->created.getEnergy() / energyScale;
	v = candidate->created.getPosition() / lengthScale;
	r.X1 = v.x;
	r.Y1 = v.y;
	r.Z1 = v.z;
	v = candidate->created.getDirection();
	r.P1x = v.x;
	r.P1y = v.y;
	r.P1z = v.z;
	
	r.weight= candidate->getWeight();

	size_t pos = 0;
	for(std::vector<Output::Property>::const_iterator iter = properties.begin();
			iter != properties.end(); ++iter)
	{
		  Variant v;
			if (candidate->hasProperty((*iter).name))
			{
				v = candidate->getProperty((*iter).name);
			}
			else
			{
				v = (*iter).defaultValue;
			}
			pos += v.copyToBuffer(&r.propertyBuffer[pos]);
	}

	#pragma omp critical 
	{
		Output::process(candidate);

		buffer.push_back(r);


		if (buffer.size() >= buffer.capacity())
		{
			KISS_LOG_DEBUG << "HDF5Output: Flush due to buffer capacity exceeded";
			flush();
		}
		else if (difftime(time(NULL), lastFlush) > 60*10)
		{
			KISS_LOG_DEBUG << "HDF5Output: Flush due to time exceeded";
			flush();
		}
	}
}

void HDF5Output::flush() const {
	const_cast<HDF5Output*>(this)->lastFlush = time(NULL);

	hsize_t n = buffer.size();

	if (n == 0)
		return;

	hid_t file_space = H5Dget_space(dset);
	hsize_t count = H5Sget_simple_extent_npoints(file_space);

	// resize dataset
	hsize_t new_size[RANK] = {count + n};
	H5Dset_extent(dset, new_size);

	// get updated filespace
	H5Sclose(file_space);
	file_space = H5Dget_space(dset);

	hsize_t offset[RANK] = {count};
	hsize_t cnt[RANK] = {n};

	H5Sselect_hyperslab(file_space, H5S_SELECT_SET, offset, NULL, cnt, NULL);
	hid_t mspace_id = H5Screate_simple(RANK, cnt, NULL);

	H5Dwrite(dset, sid, mspace_id, file_space, H5P_DEFAULT, buffer.data());

	H5Sclose(mspace_id);
	H5Sclose(file_space);

	buffer.clear();
}

std::string HDF5Output::getDescription() const  {
	return "HDF5Output";
}

} // namespace radiopropa

#endif // CRPROPA_HAVE_HDF5
