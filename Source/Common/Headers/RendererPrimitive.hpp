#include <Renderer.hpp>
#include <GLES2/gl2.h>

namespace Aura
{
	enum PRIMITIVE_TYPE
	{
		PRIMITIVE_TYPE_UNKNOWN = 0,
		PRIMITIVE_TYPE_TRIANGLE_LIST,
		PRIMITIVE_TYPE_TRIANGLE_STRIP,
		PRIMITIVE_TYPE_TRIANGLE_FAN,
		PRIMITIVE_TYPE_LINE_LIST,
		PRIMITIVE_TYPE_LINE_STRIP,
		PRIMITIVE_TYPE_LINE_LOOP
	};

	class VertexAttributes;

	class RendererPrimitive
	{
	public:
		RendererPrimitive( );
		~RendererPrimitive( );

		AUR_UINT32 Create( const AUR_MEMSIZE p_VertexCount,
			const AUR_MEMSIZE p_IndexCount, const AUR_BYTE *p_pVertices,
			const AUR_UINT16 *p_pIndices,
			const VertexAttributes &p_VertexAttributes,
			const PRIMITIVE_TYPE p_PrimitiveType );

		void Destroy( );

		AUR_UINT32 Render( );

		// Calling these will do nothing unless using the debug build
		AUR_UINT32 RenderWireframe( );
		AUR_UINT32 RenderNormals( );

	private:
		RendererPrimitive( const RendererPrimitive &p_Other );
		RendererPrimitive &operator=( const RendererPrimitive &p_Other );

#if defined AURA_BUILD_DEBUG
		GLuint			m_WireframeVertexBufferID;
		GLuint			m_WireframeIndexBufferID;
		GLuint			m_WireframeVertexArrayID;
		GLuint			m_NormalsVertexBufferID;
		GLuint			m_NormalsIndexBufferID;
		GLuint			m_NormalsVertexArrayID;
#endif // AURA_BUIDL_DEBUG
		AUR_MEMSIZE		m_PolygonCount;
		AUR_MEMSIZE		m_VertexCount;
		AUR_MEMSIZE		m_IndexCount;
		AUR_MEMSIZE		m_VertexStride;
		PRIMITIVE_TYPE	m_PrimitiveType;
		GLuint			m_VertexBufferID;
		GLuint			m_IndexBufferID;
		GLuint			m_VertexArrayID;
		GLenum			m_GLPrimitiveType;
	};
}

