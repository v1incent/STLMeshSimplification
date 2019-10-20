#pragma once
#undef min
#undef max
//#undef Handle
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <CGAL/boost/graph/graph_traits_PolyMesh_ArrayKernelT.h>

#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef OpenMesh::PolyMesh_ArrayKernelT</* MyTraits*/> Surface_mesh;
typedef boost::graph_traits<Surface_mesh>::edge_descriptor edge_descriptor;
typedef boost::graph_traits<Surface_mesh>::edge_iterator edge_iterator;

class ConstrainedEdgeMap
{

public:
    typedef boost::read_write_property_map_tag    category;
    typedef bool                                  value_type;
    typedef bool                                  reference;
    typedef edge_descriptor                       key_type;

public:
	ConstrainedEdgeMap(Surface_mesh& sm) 
		: sm_(sm)
	{
		sm.add_property(constraint);
	}
	//~ConstrainedEdgeMap(void);
    inline friend reference get( const ConstrainedEdgeMap& em, key_type e )
    {
        bool b = em.sm_.property( em.constraint, em.sm_.edge_handle( e.idx() ) );
        return b;
    }

    inline friend void put( const ConstrainedEdgeMap& em, key_type e, value_type b )
    {
        em.sm_.property( em.constraint, em.sm_.edge_handle( e.idx() ) ) = b;
    }

private:
    Surface_mesh& sm_;
    OpenMesh::EPropHandleT<bool> constraint;
};

