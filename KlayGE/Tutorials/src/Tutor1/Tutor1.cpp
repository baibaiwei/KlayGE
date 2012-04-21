#include <KlayGE/KlayGE.hpp>
#include <KlayGE/App3D.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/UI.hpp>

#include <vector>
#include <sstream>

class TutorFramework : public KlayGE::App3DFramework
{

public:
	TutorFramework();

protected:
	virtual void InitObjects();

private:
	virtual void DoUpdateOverlay();
	virtual KlayGE::uint32_t DoUpdate(KlayGE::uint32_t pass);

private:
	KlayGE::FontPtr font_;
};

int main()
{
	KlayGE::ResLoader::Instance().AddPath("../../Samples/media/Common");

	KlayGE::Context::Instance().LoadCfg("KlayGE.cfg");

	TutorFramework app;
	app.Create();
	app.Run();

	return 0;
}

TutorFramework::TutorFramework()
	: App3DFramework("Tutor1")
{
}

void TutorFramework::InitObjects()
{
	font_ = KlayGE::Context::Instance().RenderFactoryInstance().MakeFont("gkai00mp.kfont");
}

void TutorFramework::DoUpdateOverlay()
{
	std::wostringstream stream;
	stream.precision(2);
	stream << std::fixed << this->FPS() << " FPS";

	font_->RenderText(0, 0, KlayGE::Color(1, 1, 0, 1), L"Tutorial 1", 16);
	font_->RenderText(0, 18, KlayGE::Color(1, 1, 0, 1), stream.str(), 16);
}

uint32_t TutorFramework::DoUpdate(uint32_t /*pass*/)
{
	KlayGE::RenderEngine& re = KlayGE::Context::Instance().RenderFactoryInstance().RenderEngineInstance();

	re.CurFrameBuffer()->Clear(KlayGE::FrameBuffer::CBM_Color | KlayGE::FrameBuffer::CBM_Depth,
		KlayGE::Color(0.2f, 0.4f, 0.6f, 1), 1.0f, 0);

	return KlayGE::App3DFramework::URV_Need_Flush | KlayGE::App3DFramework::URV_Finished;
}
