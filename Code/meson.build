model_inc = include_directories('Model')
gui_inc = include_directories('Gui', 'Model')

blocksui_model = static_library(
  'blocksui_model',
  files(
    'Model/WidgetKind.c',
    'Model/Document.c',
    'Model/UitxtExport.c',
    'Model/UitxtImport.c',
    'Model/History.c',
    'Model/PageGuide.c',
  ),
  include_directories: model_inc,
)

blocksui_model_dep = declare_dependency(
  link_with: blocksui_model,
  include_directories: model_inc,
)

gtk4_dep = dependency('gtk4')

executable(
  'blocksui-gui',
  files(
    'Gui/Main.c',
    'Gui/Layout.c',
    'Gui/Canvas.c',
    'Gui/Palette.c',
    'Gui/PropertyPanel.c',
    'Gui/Support.c',
    'Gui/FileActions.c',
    'Gui/Icon.c',
    'Gui/PageHint.c',
    'Gui/Locale.c',
  ),
  include_directories: gui_inc,
  dependencies: [blocksui_model_dep, gtk4_dep],
  install: false,
)

executable(
  'blocksui-export-smoke',
  files('Model/ExportSmoke.c'),
  dependencies: blocksui_model_dep,
  install: false,
)

executable(
  'blocksui-import-smoke',
  files('Model/ImportSmoke.c'),
  dependencies: blocksui_model_dep,
  install: false,
)
