#include "MiMMO_VTKInterfaces.hpp"

//*****************************************************************************************************************************
// VTK_BASICMESH IMPLEMENTATION 
/*
 *	\date			31/12/2015
 *	\authors		Gallizio Federico
 * 	\authors 		Amos Tori
 *	\authors		Arpa Rocco
 *	\copyright		Copyright 2015 Optimad engineering srl. All rights reserved.
 *	\par			License:\n
 *	This class version is released under .
 *
 *	\brief Class for VTK Output of Structured Grids 
 *
 *	Class for managing output on file of uniform structured grids, based on VTK's formats (*.vtu)
 */

/*! Base Class Constructor */
VTK_BASICMESH::VTK_BASICMESH(){
	m_points = NULL;
	m_connectivity=NULL;
};
/*! Custom Constructor. Set up dimension of mesh that need to be written and its output filename and path.
 * \param[in] dir_ output directory path 
 * \param[in] name_ output filename w/out tag
 * \param[in] cod_  output codex: "ascii" or "appended"
 * \param[in] nP_   number of points in your mesh
 * \param[in] nC_   number of cells in your mesh
 * \param[in] nConn connectivity leading dimension, as sequential vector (in general 8*nC_)
 */ 
VTK_BASICMESH::VTK_BASICMESH(std::string dir_, std::string name_, std::string cod_, int nP_, int nC_, int nConn_):
VTK_UnstructuredGrid<VTK_BASICMESH>(dir_, name_) 
{
	SetCodex(cod_);
	SetDimensions(nC_, nP_, nConn_);
	m_points=NULL;
	m_connectivity=NULL;
};

/*!Basic Class Destructor */
VTK_BASICMESH::~VTK_BASICMESH(){
	m_points = NULL;
	m_connectivity = NULL;
};

/*! Link external points list and connectivity matrix to class. Data are not copied, nor stored.
 * \param[in] pp list of external points
 * \param[in] conn connectivity matrix related to pp
 */
void VTK_BASICMESH::linkData(dvecarr3E & pp, ivector2D & conn ){
	
	int typeC = 0;
	if(conn.size() >0 ){typeC = conn[0].size();}
	
	m_points = &pp;
	m_connectivity = &conn;
	//check data connection to vtk instatiated info and repair it.
	if(pp.size() != nr_points || conn.size() != nr_cells){
		SetDimensions( conn.size(), pp.size(), typeC*conn.size());
	}
};
/*! Unlink Class from points or connectivity external data*/
void VTK_BASICMESH::unlinkData(){
	m_points = NULL;
	m_connectivity = NULL;
};

/*! CRTP function to write data on file. Not suitable for a direct call. 
 * Please refer to documentation of VTK_UnstructuredGrid class in Bitpit_BASE 
 */
void VTK_BASICMESH::Flush(  fstream &str, string codex_, string name  ){
	
	int n;
	if(m_points == NULL || m_connectivity == NULL ){std::cout<<"not linked Data Structure"<<endl; return;}
	int type_ = 12;
	int lconn = 8;
	
	if((*m_connectivity)[0].size() == 4){
		type_ = 9; 
		lconn=4;
	}
	
	string indent("         ") ;
	
	if( codex_ == "ascii"){
		
		if( name == "Points" ){
			for( n=0; n<nr_points; n++) {
				flush_ascii( str, indent ) ;
				flush_ascii( str, 3, (*m_points)[n]);
				str << endl ;
			};
		};
		
		if( name == "connectivity" ){
			for( n=0; n<m_connectivity->size(); n++) {
				flush_ascii( str, indent ) ;
				flush_ascii( str, lconn, (*m_connectivity)[n]) ;
				str << endl ;
			};
		};
		
		if( name == "types" ){
			for( n=0; n<m_connectivity->size(); n++) {
				flush_ascii( str, indent ) ;
				flush_ascii( str, type_  ) ;
				str << endl ;
			};
		};
		
		if( name == "offsets" ){
			int off_(0) ;
			for( n=0; n<m_connectivity->size(); n++) {
				off_ += NumberOfElements( type_ ) ;
				flush_ascii( str, indent ) ;
				flush_ascii( str, off_  ) ;
				str << endl ;
			};
		};
		
	}else{
		if( name == "Points" ){
			for( n=0; n<nr_points; n++) {
				flush_binary( str, (*m_points)[n]);
			};
		};
		
		if( name == "connectivity" ){
			for( n=0; n<m_connectivity->size(); n++) {
				flush_binary( str, (*m_connectivity)[n]) ;
			};
		};
		
		if( name == "types" ){
			for( n=0; n<m_connectivity->size(); n++) {
				flush_binary( str, type_  ) ;
			};
		};
		
		if( name == "offsets" ){
			int off_(0) ;
			for( n=0; n<m_connectivity->size(); n++) {
				off_ += NumberOfElements( type_ ) ;
				flush_binary( str, off_  ) ;
			};
		};
		
	}
	
	return ;
}; //CRTP

//*****************************************************************************************************************************
// VTK_BASICCLOUD IMPLEMENTATION 
/*
 *	\date			31/12/2015
 *	\authors		Gallizio Federico
 * 	\authors 		Amos Tori
 *	\authors		Arpa Rocco
 *	\copyright		Copyright 2015 Optimad engineering srl. All rights reserved.
 *	\par			License:\n
 *	This class version is released under .
 *
 *	\brief Class for VTK Output of Point Clouds
 *
 *	Class for managing output on file of point clouds, based on VTK's formats (*.vtu)
 */

/*! Basic class Constructor */
VTK_BASICCLOUD::VTK_BASICCLOUD(){
	m_points = NULL;
};

/*! Custom Constructor. Set up dimension of cloud points that need to be written and its output filename and path.
 * \param[in] dir_ output directory path 
 * \param[in] name_ output filename w/out tag
 * \param[in] cod_  output codex: "ascii" or "appended"
 * \param[in] nP_   number of points in your mesh
 */ 
VTK_BASICCLOUD::VTK_BASICCLOUD(std::string dir_, std::string name_, std::string cod_, int nP_):
VTK_UnstructuredGrid<VTK_BASICCLOUD>(dir_, name_)  
{
	SetCodex(cod_);
	SetDimensions(nP_, nP_, nP_);
	m_points=NULL;
};

/*! Basic Class Destructor */
VTK_BASICCLOUD::~VTK_BASICCLOUD(){
	m_points=NULL;
};

/*! Link external point lists to class
 * \param[in] pp external point cloud
 */
void VTK_BASICCLOUD::linkData(dvecarr3E & pp){
	
	m_points = &pp;
	//check data connection to vtk instatiated info and repair it.
	if(pp.size() != nr_points){
		SetDimensions( pp.size(), pp.size(), pp.size());
	}
};

/*! Unlink preexistent data from your class */
void VTK_BASICCLOUD::unlinkData(){
	m_points = NULL;
};

/*! CRTP function to write data on file. Not suitable for a direct call. 
 * Please refer to documentation of VTK_UnstructuredGrid class in Bitpit_BASE 
 */
void VTK_BASICCLOUD::Flush(  fstream &str, string codex_, string name  ){
	
	int n;
	if(m_points == NULL ){std::cout<<"not linked Data Structure"<<endl; return;}
	
	string indent("         ") ;
	
	if( codex_ == "ascii"){
		
		if( name == "Points" ){
			for( n=0; n<m_points->size(); n++) {
				flush_ascii( str, indent ) ;
				flush_ascii( str, 3, (*m_points)[n]);
				str << endl ;
			};
		};
		
		if( name == "connectivity" ){
			for( n=0; n<m_points->size(); n++) {
				flush_ascii( str, indent ) ;
				flush_ascii( str, n) ;
				str << endl ;
			};
		};
		
		if( name == "types" ){
			int type_(1) ;
			for( n=0; n<m_points->size(); n++) {
				flush_ascii( str, indent ) ;
				flush_ascii( str, type_  ) ;
				str << endl ;
			};
		};
		
		if( name == "offsets" ){
			int off_(0) ;
			for( n=0; n<m_points->size(); n++) {
				off_ += NumberOfElements( 1 ) ;
				flush_ascii( str, indent ) ;
				flush_ascii( str, off_  ) ;
				str << endl ;
			};
		};
		
	}else{
		if( name == "Points" ){
			for( n=0; n<m_points->size(); n++) {
				flush_binary( str, (*m_points)[n]);
			};
		};
		
		if( name == "connectivity" ){
			for( n=0; n<m_points->size(); n++) {
				flush_binary( str, n) ;
			};
		};
		
		if( name == "types" ){
			int type_(1) ;
			for( n=0; n<m_points->size(); n++) {
				flush_binary( str, type_  ) ;
			};
		};
		
		if( name == "offsets" ){
			int off_(0) ;
			for( n=0; n<m_points->size(); n++) {
				off_ += NumberOfElements( 1 ) ;
				flush_binary( str, off_  ) ;
			};
		};
	}
	return ;
}; //CRTP
