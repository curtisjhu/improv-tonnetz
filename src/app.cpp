#include "main.h"

class ImprovTonnetzApplication  : public juce::JUCEApplication {
	public:
		ImprovTonnetzApplication() {}

		const juce::String getApplicationName() override       { return juce::String("ImprovTonnetz"); }
		const juce::String getApplicationVersion() override    { return juce::String("0.1.1"); }
		bool moreThanOneInstanceAllowed() override             { return true; }
		void initialise (const juce::String& commandLine) override {
			mainWindow.reset (new MainWindow (getApplicationName()));
		}

		void shutdown() override {
			mainWindow = nullptr; // delete window
		}

		void systemRequestedQuit() override {
			quit();
		}

		void anotherInstanceStarted (const juce::String& commandLine) override {}

		class MainWindow : public juce::DocumentWindow {
			public:
				MainWindow (juce::String name)
					: DocumentWindow (name,
									juce::Desktop::getInstance().getDefaultLookAndFeel()
																.findColour (juce::ResizableWindow::backgroundColourId),
									DocumentWindow::allButtons)
				{
					setUsingNativeTitleBar (true);
					setContentOwned (new MainContentComponent(), true);

				#if JUCE_IOS || JUCE_ANDROID
					setFullScreen (true);
				#else
					setResizable (true, true);
					centreWithSize (getWidth(), getHeight());
				#endif

					setVisible (true);
				}

				void closeButtonPressed() override {
					JUCEApplication::getInstance()->systemRequestedQuit();
				}
			private:
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
		};

	private:
		std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (ImprovTonnetzApplication)