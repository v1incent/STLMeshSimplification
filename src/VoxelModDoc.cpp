
#include "ConstrainedEdgeMap.h"


void OnMeshSimp()
{
	if(!IsLoadSTL){
		AfxMessageBox(_T("请导入STL模型！"));
		return;
	}

	CFileDialog dlg(FALSE, L"*.stl",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                 L"STL Files (*.stl )|*.stl;||", NULL);

	CString SHAREPATHValue;
	SHAREPATHValue.GetEnvironmentVariable (L"CSF_OCCTDataPath");
	CString initdir = (SHAREPATHValue + "\\stl");

	dlg.m_ofn.lpstrInitialDir = initdir;

	CMeshSimpDialog meshSimpDia;
	if(meshSimpDia.DoModal() == IDOK)
	{
  
		if (dlg.DoModal() == IDOK)  
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
			static_cast<CMainFrame*>(AfxGetMainWnd())->m_wndOutput.AddMessage(CString("正在处理STL模型，请稍后..."));
			static_cast<CMainFrame*>(AfxGetMainWnd())->m_wndOutput.UpdateWindow();
			TCollection_AsciiString aFileName ((const wchar_t* )dlg.GetPathName());

			Surface_mesh surface_mesh;
			ConstrainedEdgeMap constraints_map( surface_mesh );

			if(!OpenMesh::IO::read_mesh( surface_mesh, m_stlPathName_AsciiString.ToCString())){
				AfxMessageBox(_T("数据读取失败。文件名及其路径中请勿包含中文！"));
				return;
			}

			// For the pupose of the example we mark 10 edges as constrained edges
			edge_iterator b, e;
			int count = 0;
			for( boost::tie( b, e ) = edges( surface_mesh ); b != e; ++b )
			{
				put( constraints_map, *b, ( count++ < 100 ) );
			}
			// This is a stop predicate (defines when the algorithm terminates).
			// In this example, the simplification stops when the number of undirected edges
			// left in the surface mesh drops below the specified number (1000)
			CGAL::Surface_mesh_simplification::Count_stop_predicate<Surface_mesh> stop( surface_mesh.n_faces() * meshSimpDia.m_MeshSimpRatio );

			// This the actual call to the simplification algorithm.
			// The surface mesh and stop conditions are mandatory arguments.

			int r = CGAL::Surface_mesh_simplification::edge_collapse
					( surface_mesh
					  , stop
					  , CGAL::parameters::halfedge_index_map  ( get( CGAL::halfedge_index  , surface_mesh ) )
					  .vertex_point_map( get( boost::vertex_point, surface_mesh ) )
					  .edge_is_constrained_map( constraints_map )
					);

			surface_mesh.garbage_collection();
			OpenMesh::IO::write_mesh( surface_mesh, aFileName.ToCString() );

			OnImportStl(dlg.m_ofn.lpstrFile, dlg.m_ofn.lpstrFileTitle);

			static_cast<CMainFrame*>(AfxGetMainWnd())->m_wndOutput.AddMessage(CString("成功简化STL模型！"));
			static_cast<CMainFrame*>(AfxGetMainWnd())->m_wndOutput.UpdateWindow();
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
	}
}

