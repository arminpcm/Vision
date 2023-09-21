// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include "Application/Example/ExampleComponentLibrary/ExampleComponentLibrary.h"
#include "Library/Component/Component.h"
#include "Library/Component/ComponentImpl.h"

using vision::component::Component;

constexpr float FREQUENCY = 10.0;

int main(int argc, char* argv[]) {
    std::shared_ptr<char**> shared_argv(&argv);
    Component<example::Person, uint8_t> example_component(argc,
                                                          shared_argv,
                                                          vision::component::OnInit,
                                                          vision::component::OnUpdate);

    example_component.Run(FREQUENCY);
}