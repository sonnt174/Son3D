#include "ISystemDrawing.h"

bsp_node *ISystemDrawing::s_world = NULL;
son_matrix4x4_t ISystemDrawing::m_modelViewMatrix;
son_vector3d_t ISystemDrawing::eye;

ISystemDrawing::ISystemDrawing(void)
{

}


ISystemDrawing::~ISystemDrawing(void)
{
	bsp_clear(s_world);
}