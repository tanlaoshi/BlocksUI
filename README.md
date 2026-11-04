# BlocksUI

Ubuntu 主机侧 **Blocks 应用 UI 设计器**：拖控件 → 导出 `.uitxt` →（`uitxt2h`）→ 编进 Guest ELF。

- **产品名**：BlocksUI  
- **本仓路径**：`~/BlocksUI`（独立 Git，**不**在 Blocks 树内）  
- **对表规范**：Blocks 仓 `Bingo/App/UI/`（布局格式 / 多页面 / 拖控件手册）  
- **构建**：Meson + GTK4（对齐 HotSend）  
- **活 ★**：[`Docs/05-当前星.md`](Docs/05-当前星.md)（勿认 Blocks 板侧 ★）

```bash
meson setup Build/meson
meson compile -C Build/meson
./Build/meson/Code/blocksui-gui
./Tools/install-desktop.sh   # 图标 + 应用菜单 / 桌面快捷方式
```

入门：[`Docs/00-概述.md`](Docs/00-概述.md) → [`Docs/02-目录与构建.md`](Docs/02-目录与构建.md) → [`Docs/05-当前星.md`](Docs/05-当前星.md)。
