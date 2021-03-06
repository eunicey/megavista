#ifndef _GRAYVOLUME_H_
#define _GRAYVOLUME_H_

#include "parametersmap.h"

/// Voxel data operating class
/// Used to load .class and .gray files

class CGrayVolume
{
public:
	CGrayVolume();
	virtual ~CGrayVolume();

	/// Load from .gray
	bool	LoadGray(const wxString &strFileName, CParametersMap &paramsOut);
	/// Load from .class
	bool	LoadClass(const wxString &strFileName, CParametersMap &paramsIn, CParametersMap &paramsOut);
	///
	bool	CreateFromArray(CParametersMap &paramsIn, CParametersMap &paramsOut);
	void	Free();

	/// Polygonize voxel data
	vtkPolyData	*BuildMesh();

private:
	/// .Gray file header.
	struct GrayHeader{
		int	iSizeX,	///< dimensions in voxels
			iSizeY,	///< dimensions in voxels
			iSizeZ;	///< dimensions in voxels
		int	nNodes,	///< voxels count
			nEdges;	///< number of graph edges
	};
	/// Volume graph node
	struct	GraphNode
	{
		int	x,	///< Voxel X
			y,	///< Voxel Y
			z;	///< Voxel Z
		int	nNeighbours;	///< Neighbours/edges count
		int iNeighbOffsOrWhat;
		int	iLayer;	///< Gray matter layer
	};

	/// .class file header info
	struct ClassHeader
	{
		int		iVerMajor;
		int		iVerMinor;
		int		Bounds[6];
		int		Size[3];
		char	csf;
		char	gray;
		char	white;
	};

	struct	GraphEdge
	{
		int	neighbours[26];	///< Indicies of nearst voxels
	};

	/// Here we hold SizeX*SizeY*SizeZ voxels with values set to 
	/// 0 where voxel exists and 2 if point is empty.
	vtkStructuredPoints	*pClassData;

	bool	bLoaded;

	/// Reads text header and sets iHeaderSize to offset to binary data
	bool ReadClassHeader(FILE *f, ClassHeader &hdr, int *piHeaderSize);
	/// Swaps vertex order in mesh triangles. No strips are processed.
	/// Marching cubes output contains no normals, so only points are swapped.
	void ReverseTriangles(vtkPolyData *pPD);
};

#endif //_GRAYVOLUME_H_
