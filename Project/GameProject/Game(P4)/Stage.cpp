#include "Stage.h"

Stage::Stage()
:Base(BaseType::eField)
{
	m_model = COPY_RESOURCE("Stage", CModelObj);
}

void Stage::Update()
{

	
	

}

void Stage::Render()
{
	m_model.SetScale(1, 1, 1);
	m_model.Render();
}
