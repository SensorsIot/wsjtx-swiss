# WSJT-SWISS Build Notes

## GitHub Actions Workflows

| Workflow | Trigger | Purpose |
|----------|---------|---------|
| **Build WSJT-X Windows Installer** | `v*` tags, manual | Production builds |
| **Build NSIS Test** | manual only | Test NSIS/installer changes |

All builds have to ceate a zip file as artifact

**Build NSIS Test** has to create an NSIS that

- checks if ini file is present in WSJTX-SWISS
- if not, ask the customer if he wants to copy the ini file
- Default path for the ini file is C:\WSJTX
- The user can change this path
- When accepted, the ini file is copied
- 
