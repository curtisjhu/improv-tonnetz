#include "main.h"

void Improv::setup()
{
	auto ctx = audio::master();

	mGen = ctx->makeNode( new audio::GenSineNode );
	mGain = ctx->makeNode( new audio::GainNode );

	mGen->setFreq( 130.8 );
	mGain->setValue( 0.5f );

	mGen >> mGain >> ctx->getOutput();
	mGen->enable();
	ctx->enable();

	log::makeLogger<log::LoggerFileRotating>( app::getAppPath() / "logs", "cinder.%Y.%m.%d.log", true, []( const fs::path& path ) {
		ci::limitDirectoryFileCount( path.parent_path(), 10 );
	});

	time.start();

	render(";)");
}

void Improv::render(string text) {
	mText = TextBox().text(text).font(Font( "Times New Roman", 24 )).size(vec2( 100, 100 ));
	mText.setBackgroundColor(Color(1, 1, 1));
	mText.setColor(Color(0, 0, 0));
	mTextTexture = gl::Texture2d::create(mText.render());
}

void Improv::updateNote() {
	nextNoteChange = tonnetz.perlinNoteWalk();
	float freq = tonnetz.getFreq();
	// render(to_string(freq));
	mGen->setFreq(freq);
}

void Improv::updateChord() {
	
}

void Improv::update() {
	if (time.getSeconds() > nextNoteChange) {
		updateNote();
	}
}

void Improv::draw()
{
	gl::clear( Color( 1, 1, 1 ) );
	if (mTextTexture)
		gl::draw(mTextTexture);
}

CINDER_APP( Improv, RendererGl, []( App::Settings *settings ) {
	settings->setMultiTouchEnabled( false );
} )