#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"
#include "cinder/audio/audio.h"
#include "cinder/CinderResources.h"
#include "AudioDrawUtils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImprovApp : public App {
	public:
		void setup() override;
		void update();
		void draw() override;
		void keyDown(KeyEvent event);

		audio::VoiceRef playback;
};

void ImprovApp::setup() {
	audio::SourceFileRef sourceFile = audio::load(app::loadAsset("sound.wav"));
	playback = audio::Voice::create(sourceFile);

	playback->start();
}

CINDER_APP( ImprovApp, RendererGl( RendererGl::Options().msaa( 8 ) ), []( App::Settings *settings ) {
	settings->setMultiTouchEnabled( false );
} )