#include "main.h"

void Improv::setup() {
	auto ctx = audio::master();

	note.wave = ctx->makeNode( new audio::GenSineNode );
	note.gain = ctx->makeNode( new audio::GainNode );

	note.wave->setFreq( 440 );
	note.gain->setValue( 0.5f );

	chord.one = ctx->makeNode( new audio::GenSineNode );
	// chord.three = ctx->makeNode( new audio::GenSineNode );
	// chord.five = ctx->makeNode( new audio::GenSineNode );
	// chord.gain = ctx->makeNode( new audio::GainNode );

	// chord.one->setFreq(130.8);
	// chord.three->setFreq(196.8);
	// chord.five->setFreq(155.5);
	// chord.gain->setValue(0.5f);

	// chord.one >> chord.three >> chord.five >> chord.gain >> note.wave >> note.gain >> ctx->getOutput();
	chord.one >> note.wave >> note.gain >> ctx->getOutput();
	note.wave->enable();
	chord.one->enable();
	// chord.three->enable();
	// chord.five->enable();
	ctx->enable();

	log::makeLogger<log::LoggerFileRotating>( app::getAppPath() / "logs", "cinder.%Y.%m.%d.log", true, []( const fs::path& path ) {
		ci::limitDirectoryFileCount( path.parent_path(), 10 );
	});

	time.start();
}

void Improv::render(string text) {
	mText = TextBox().text(text).font(Font( "Times New Roman", 24 )).size(vec2( 100, 100 ));
	mText.setBackgroundColor(Color(1, 1, 1));
	mText.setColor(Color(0, 0, 0));
	mTextTexture = gl::Texture2d::create(mText.render());
}

void Improv::updateNote() {
	tuple<float, float, float> n = tonnetz.perlinNoteWalk();
	noteDuration = std::get<0>(n);
	float freq = std::get<1>(n);
	float gain = std::get<2>(n);

	note.wave->setFreq(freq);
	note.gain->setValue(gain);
}

void Improv::updateChord() {
	tuple<float, Triad, float> c = tonnetz.classicalChordWalk();
	chordDuration = std::get<0>(c);
	Triad chordTriad = std::get<1>(c);
	float chordGain = std::get<2>(c);

	chord.one->setFreq(chordTriad.oneFreq);
	chord.three->setFreq(chordTriad.threeFreq);
	chord.five->setFreq(chordTriad.fiveFreq);
	chord.gain->setValue(chordGain);
}

void Improv::update() {
	float currentTime = time.getSeconds();
	if (currentTime - lastNoteTime > noteDuration) {
		lastNoteTime = currentTime;
		updateNote();
	}
	// if (currentTime - lastChordTime > chordDuration) {
	// 	lastChordTime = currentTime;
	// 	updateChord();
	// }
}

void Improv::draw() {
	gl::clear( Color( 1, 1, 1 ) );
	if (mTextTexture)
		gl::draw(mTextTexture);
}

CINDER_APP( Improv, RendererGl, []( App::Settings *settings ) {
	settings->setMultiTouchEnabled( false );
} )