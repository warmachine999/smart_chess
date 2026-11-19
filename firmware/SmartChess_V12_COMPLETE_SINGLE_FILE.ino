#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>
#include <Preferences.h>
#include <pgmspace.h>

// ============================================================
// EMBEDDED SMART CHESS V12 APP (single-file build)
// ============================================================
const char SMART_CHESS_APP_HTML[] PROGMEM =
"\n"
"<!doctype html>\n"
"<html lang=\"en\">\n"
"<head>\n"
"<meta charset=\"utf-8\">\n"
"<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,viewport-fit=cover\">\n"
"<meta name=\"theme-color\" content=\"#05080b\">\n"
"<title>Smart Chess V12 // Prime Control</title>\n"
"<style>\n"
":root{\n"
"  --bg:#05080b;--panel:#0a1015;--panel2:#0d151b;--panel3:#111c23;\n"
"  --line:#1b2b33;--line2:#263c46;--text:#edf7fa;--muted:#8298a1;\n"
"  --cyan:#27d8e9;--cyan2:#90f7ff;--gray:#aab5bb;--gold:#f2c85b;--red:#ff5d68;--violet:#a98cff;--green:#4be3a4;\n"
"  --shadow:0 24px 70px rgba(0,0,0,.42),0 2px 9px rgba(0,0,0,.22);\n"
"}\n"
"*{box-sizing:border-box}\n"
"html,body{margin:0;min-height:100%;background:var(--bg);color:var(--text);font-family:Inter,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,\"Segoe UI\",sans-serif}\n"
"body{background-image:radial-gradient(circle at 20% 0%,rgba(39,216,233,.08),transparent 30%),radial-gradient(circle at 90% 12%,rgba(169,140,255,.07),transparent 25%),linear-gradient(rgba(255,255,255,.018) 1px,transparent 1px),linear-gradient(90deg,rgba(255,255,255,.014) 1px,transparent 1px);background-size:auto,auto,44px 44px,44px 44px;}\n"
"button,input,select,textarea{font:inherit}button{color:inherit}\n"
".app-shell{min-height:100vh;display:grid;grid-template-columns:252px minmax(0,1fr)}\n"
".sidebar{position:sticky;top:0;height:100vh;display:flex;flex-direction:column;overflow-y:auto;scrollbar-width:thin;padding:22px 14px 14px;background:rgba(4,8,11,.94);border-right:1px solid var(--line);backdrop-filter:blur(18px);z-index:50}\n"
".brand-block{display:flex;align-items:center;gap:12px;padding:0 8px 16px}\n"
".brand-symbol{width:38px;height:38px;display:grid;grid-template-columns:1fr 1fr;grid-template-rows:1fr 1fr;gap:3px;transform:rotate(45deg)}\n"
".brand-symbol span{border:1px solid rgba(144,247,255,.6);background:linear-gradient(145deg,rgba(39,216,233,.35),rgba(39,216,233,.03));box-shadow:0 0 15px rgba(39,216,233,.14)}\n"
".brand-symbol span:nth-child(2),.brand-symbol span:nth-child(3){background:rgba(255,255,255,.025)}\n"
".brand-name{font-size:14px;font-weight:900;letter-spacing:.18em}.brand-meta{margin-top:4px;font-size:7px;letter-spacing:.19em;color:#7b929c}\n"
".version-mark{margin:0 8px 14px;padding:8px 10px;border:1px solid rgba(39,216,233,.17);border-radius:8px;background:linear-gradient(90deg,rgba(39,216,233,.08),rgba(39,216,233,0));color:#91edf4;font-size:8px;font-weight:800;letter-spacing:.16em}\n"
".side-live{display:flex;gap:10px;align-items:center;margin:0 8px 15px;padding:11px 10px;background:#071116;border:1px solid var(--line);border-radius:11px}.side-live b{display:block;font-size:10px}.side-live span{display:block;margin-top:3px;color:#66818b;font-size:7px;letter-spacing:.11em}.dot{display:inline-block;width:7px;height:7px;border-radius:50%;background:#3c4d55;box-shadow:0 0 0 4px rgba(255,255,255,.018)}.dot.on{background:var(--green);box-shadow:0 0 0 4px rgba(75,227,164,.09),0 0 16px rgba(75,227,164,.38)}.dot.bad{background:var(--red);box-shadow:0 0 0 4px rgba(255,93,104,.08),0 0 16px rgba(255,93,104,.26)}.dot.warn{background:var(--gold);box-shadow:0 0 0 4px rgba(242,200,91,.08),0 0 16px rgba(242,200,91,.22)}.dot.blue{background:#55a8ff;box-shadow:0 0 0 4px rgba(85,168,255,.08),0 0 16px rgba(85,168,255,.22)}.dot.idle{background:#57676d}\n"
".nav{display:grid;gap:5px;overflow-y:auto;max-height:56vh;padding-right:2px;scrollbar-width:thin}.nav-item{border:1px solid transparent;background:transparent;padding:10px 11px;border-radius:9px;display:grid;grid-template-columns:27px 1fr;text-align:left;align-items:center;gap:8px;cursor:pointer;color:#7e969f;transition:.18s}.nav-item:hover{background:#081015;border-color:#14252c;color:#dfecee}.nav-item.active{background:linear-gradient(90deg,rgba(39,216,233,.11),rgba(39,216,233,.035));border-color:rgba(39,216,233,.24);color:#e8fbfd;box-shadow:inset 2px 0 0 var(--cyan)}.nav-index{font-size:7px;letter-spacing:.12em;color:#50656d}.nav-item.active .nav-index{color:var(--cyan)}.nav-item span:last-child{font-size:9px;font-weight:750;letter-spacing:.04em}\n"
".sidebar-footer{margin-top:auto;padding:12px 8px 2px;border-top:1px solid var(--line)}.hardware-badge{padding:10px;background:#071015;border:1px solid var(--line);border-radius:10px}.hardware-badge b{display:block;font-size:8px;letter-spacing:.08em}.hardware-badge span{display:block;margin-top:4px;color:#667e87;font-size:6px;letter-spacing:.11em}.side-footline{display:flex;align-items:center;gap:7px;margin-top:9px;color:#617982;font-size:7px;letter-spacing:.08em}.side-footline b{color:#8cecf2}\n"
".main{min-width:0;padding:0 24px 28px}.topbar{position:sticky;top:0;z-index:40;display:grid;grid-template-columns:auto auto 1fr auto;align-items:center;gap:14px;padding:17px 0 14px;background:linear-gradient(var(--bg) 74%,rgba(5,8,11,.78) 86%,transparent);backdrop-filter:blur(10px)}.mobile-menu{display:none;width:40px;height:40px;border:1px solid var(--line);border-radius:10px;background:#081015}.breadcrumb{font-size:8px;color:#69818a;letter-spacing:.15em}.topbar-title{display:flex;align-items:center;gap:8px}.topbar h1{font-size:19px;margin:2px 0 0;letter-spacing:-.02em}.tiny-version{font-size:7px;padding:5px 6px;border:1px solid var(--line);border-radius:6px;color:#75dbe3;letter-spacing:.12em}.topbar-right{display:flex;align-items:center;gap:7px;justify-self:end}.status-pill{display:flex;align-items:center;gap:6px;padding:8px 9px;border:1px solid var(--line);border-radius:9px;background:#081015;font-size:7px;color:#657d86;letter-spacing:.08em}.status-pill b{color:#cfe6eb}.status-pill.optional{border-color:#243039}.icon-button{width:37px;height:37px;border:1px solid var(--line2);border-radius:9px;background:#0a1217;cursor:pointer;font-size:18px}.icon-button:hover{border-color:rgba(39,216,233,.35);color:var(--cyan)}\n"
".page{display:none}.page.active{display:block}.eyebrow{font-size:7px;letter-spacing:.18em;color:#607780;font-weight:850}.accent-text{color:#7eeef3}.muted{color:var(--muted);font-size:9px;line-height:1.65}.page-headline{display:flex;justify-content:space-between;gap:18px;align-items:flex-end;margin:12px 0 17px}.page-headline h2{margin:7px 0 5px;font-size:29px;letter-spacing:-.04em;line-height:1.08}.page-headline p{margin:0;max-width:780px;color:#79919a;font-size:10px;line-height:1.65}.page-actions{display:flex;align-items:center;gap:8px;flex-wrap:wrap}.card{background:linear-gradient(180deg,rgba(13,21,27,.96),rgba(8,13,17,.96));border:1px solid var(--line);border-radius:15px;box-shadow:var(--shadow);position:relative;overflow:hidden}.card:before{content:\"\";position:absolute;inset:0;pointer-events:none;background:linear-gradient(120deg,rgba(255,255,255,.025),transparent 23%,transparent 75%,rgba(255,255,255,.012))}.card>*,.card .section-head{position:relative}.card{padding:15px}.section-head{display:flex;align-items:flex-start;justify-content:space-between;gap:10px}.section-head h3{margin:5px 0 0;font-size:13px;letter-spacing:-.015em}.head-chips{display:flex;gap:6px;flex-wrap:wrap}.chip{padding:6px 8px;border:1px solid var(--line2);border-radius:999px;background:#071015;color:#7d949d;font-size:7px;letter-spacing:.1em;font-weight:800;white-space:nowrap}.chip.accent{border-color:rgba(39,216,233,.28);color:#8deff5;background:rgba(39,216,233,.05)}.chip.warn{border-color:rgba(244,203,104,.28);color:#e7cb7b;background:rgba(244,203,104,.05)}.signal{font-size:7px;letter-spacing:.1em;color:#6e838c}\n"
"button,.button-link{appearance:none;border:1px solid var(--line2);background:#0a1419;color:#dff3f6;padding:9px 11px;border-radius:9px;cursor:pointer;transition:.16s;font-size:8px;font-weight:800;letter-spacing:.06em;box-shadow:none}button:disabled{opacity:.45;cursor:not-allowed}.primary,.secondary,.ghost,.small-btn,.quick,.anim-btn,.replay-row button,.paint-swatch,.frame-tabs button,.button-link{border:1px solid var(--line2);background:#0a1419;padding:9px 11px;border-radius:9px;cursor:pointer;transition:.16s;font-size:8px;font-weight:800;letter-spacing:.06em}.primary{background:linear-gradient(135deg,#26bfcf,#23dce9);border-color:#36e5ef;color:#031015;box-shadow:0 8px 26px rgba(39,216,233,.13)}.primary:hover{transform:translateY(-1px);box-shadow:0 12px 32px rgba(39,216,233,.2)}.secondary:hover,.ghost:hover,.small-btn:hover,.quick:hover,.anim-btn:hover,.replay-row button:hover{border-color:rgba(39,216,233,.36);color:#eaffff;background:#0d1b21}.ghost{background:transparent}.small-btn{padding:7px 9px;font-size:7px}.full{width:100%;margin-top:10px}.row{display:flex;gap:7px;flex-wrap:wrap;margin-top:10px}.row>*{flex:1}.message{border:1px solid var(--line);border-radius:10px;padding:10px 11px;font-size:8px;line-height:1.6}.message.info{background:#071015;color:#91a8af}.message.success{background:rgba(75,227,164,.05);border-color:rgba(75,227,164,.18);color:#84dfba}.message.warning{background:rgba(242,200,91,.06);border-color:rgba(242,200,91,.18);color:#dfc578}.message.error{background:rgba(255,93,104,.06);border-color:rgba(255,93,104,.2);color:#ff9198}\n"
".command-hero{display:grid;grid-template-columns:minmax(0,1.15fr) minmax(360px,.85fr);min-height:365px;margin-bottom:15px;border:1px solid var(--line);border-radius:18px;overflow:hidden;box-shadow:var(--shadow);background:radial-gradient(circle at 84% 50%,rgba(39,216,233,.08),transparent 28%),linear-gradient(120deg,#081015,#060b0f 60%,#0a1117)}.hero-copy{padding:38px 38px 34px}.hero-copy h2{margin:10px 0 12px;font-size:45px;line-height:1.02;letter-spacing:-.055em;max-width:660px}.hero-copy p{max-width:700px;color:#7c939b;font-size:10px;line-height:1.75}.hero-actions{display:flex;gap:8px;flex-wrap:wrap;margin-top:22px}.hero-console{position:relative;min-height:365px;display:grid;place-items:center;overflow:hidden}.console-ring{position:absolute;border:1px solid rgba(39,216,233,.18);border-radius:50%;box-shadow:0 0 80px rgba(39,216,233,.05)}.ring-a{width:390px;height:390px}.ring-b{width:285px;height:285px;border-color:rgba(169,140,255,.16)}.ring-c{width:185px;height:185px;border-color:rgba(242,200,91,.13)}.console-core{width:136px;height:136px;border-radius:28px;display:grid;place-items:center;align-content:center;gap:4px;background:linear-gradient(145deg,#12252d,#091216 70%);border:1px solid rgba(39,216,233,.32);box-shadow:0 0 60px rgba(39,216,233,.13),inset 0 0 50px rgba(39,216,233,.06);z-index:2;text-align:center}.console-core strong{font-size:32px;letter-spacing:-.05em}.console-core span,.console-core em{font-style:normal;font-size:6px;letter-spacing:.16em;color:#70a1a8}.console-core em{color:#b7d2d7}.console-tag{position:absolute;padding:6px 8px;border:1px solid var(--line2);border-radius:8px;background:#071015;color:#7f959c;font-size:6px;letter-spacing:.14em}.tag-a{top:22%;left:8%}.tag-b{top:16%;right:9%}.tag-c{bottom:19%;right:11%}\n"
".cockpit-grid{display:grid;grid-template-columns:minmax(0,1.35fr) minmax(320px,.65fr);gap:14px}.cockpit-board{grid-row:span 2}.board-frame{display:grid;place-items:center;padding:16px 0 10px}.chess-board{width:min(100%,690px);height:auto;aspect-ratio:1 / 1;display:grid;grid-template-columns:repeat(8,minmax(0,1fr));grid-template-rows:repeat(8,minmax(0,1fr));grid-auto-flow:row;overflow:hidden;border-radius:12px;border:5px solid #0e181d;background:#05090c;box-shadow:0 24px 60px rgba(0,0,0,.44),inset 0 0 22px rgba(255,255,255,.025)}.sq{position:relative;width:100%;height:100%;min-width:0;min-height:0;aspect-ratio:1 / 1 !important;border:0;padding:0;margin:0;display:grid;place-items:center;line-height:1;cursor:pointer;font-size:clamp(17px,3.3vw,36px);font-weight:500;transition:.1s}.sq.light{background:#d7e0e4;color:#142026}.sq.dark{background:#657680;color:#f5f8f9}.sq:hover{filter:brightness(1.08)}.piece{position:relative;z-index:2;line-height:1;font-size:1.15em;font-weight:700;-webkit-text-stroke:.35px rgba(0,0,0,.3);text-shadow:0 1px 0 rgba(255,255,255,.35),0 3px 0 rgba(0,0,0,.25),0 6px 12px rgba(0,0,0,.35),0 0 16px currentColor}.cyanPiece{color:#effffb}.grayPiece{color:#182226;text-shadow:0 1px 0 rgba(255,255,255,.55),0 2px 0 rgba(0,0,0,.42),0 6px 12px rgba(0,0,0,.35)}.physicalMagnet{font-size:clamp(16px,2.4vw,28px);color:#d9f9fb;text-shadow:0 0 16px rgba(39,216,233,.7),0 2px 5px rgba(0,0,0,.45);z-index:2}.sq.selected{box-shadow:inset 0 0 0 4px var(--cyan),inset 0 0 22px rgba(39,216,233,.35)}.sq.legal:after,.sq.capture:after{content:\"\";position:absolute;width:23%;height:23%;border-radius:50%;background:rgba(144,247,255,.85);box-shadow:0 0 15px rgba(144,247,255,.35)}.sq.capture:after{width:75%;height:75%;background:transparent;border:3px solid rgba(255,195,87,.78);box-shadow:0 0 18px rgba(255,195,87,.22)}.sq.last:before{content:\"\";position:absolute;inset:0;background:rgba(242,200,91,.13)}.sq.check{box-shadow:inset 0 0 0 4px rgba(255,93,104,.9),inset 0 0 28px rgba(255,93,104,.23)}.sq.mismatch{box-shadow:inset 0 0 0 4px var(--red),inset 0 0 25px rgba(255,93,104,.25)}.coord{position:absolute;right:4px;bottom:3px;font-size:6px;opacity:.48;letter-spacing:.04em;font-weight:800}.board-legend{display:flex;gap:13px;flex-wrap:wrap;align-items:center;padding-top:6px;color:#71878f;font-size:7px;letter-spacing:.04em}.board-legend span{display:inline-flex;align-items:center;gap:5px}.legend-dot{width:6px;height:6px;border-radius:50%;display:inline-block}.legend-dot.cyan{background:var(--cyan)}.legend-dot.gray{background:#9faab0}.legend-dot.guide{background:#4a9be8}.legend-dot.warn{background:var(--red)}.board-legend span:last-child{margin-left:auto}\n"
".metric-grid.prime{display:grid;grid-template-columns:1fr 1fr;gap:7px;margin-top:13px}.metric{padding:10px;border:1px solid var(--line);border-radius:10px;background:#071015;min-width:0}.metric span{display:block;font-size:6px;letter-spacing:.09em;color:#60757d;text-transform:uppercase}.metric b{display:block;margin-top:5px;font-size:11px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}.metric.hot{border-color:rgba(39,216,233,.18)}.metric.hot b{color:#8deff5}.event-panel{display:grid;grid-template-columns:1fr auto;grid-template-rows:auto 1fr;gap:3px 10px;margin-top:9px;padding:11px;border:1px solid var(--line);border-radius:10px;background:#060d11}.event-kicker{grid-column:1/-1;font-size:6px;color:#60767f;letter-spacing:.14em}.event-main{font-size:9px;font-weight:750;align-self:center}.event-square{font-size:17px;font-weight:900;color:var(--cyan);align-self:center}.home-grid-placeholder{display:none}.quick-grid{display:grid;grid-template-columns:1fr 1fr;gap:7px;margin-top:13px}.quick{text-align:left;padding:10px;background:#081116}.quick b{display:block;font-size:8px}.quick span{display:block;margin-top:4px;color:#6e858d;font-size:6px;font-weight:500}.quick.danger-soft{border-color:rgba(255,93,104,.18)}.quick.rainbow{background:linear-gradient(135deg,rgba(255,93,104,.12),rgba(242,200,91,.08),rgba(75,227,164,.08),rgba(39,216,233,.1),rgba(169,140,255,.1))}.quick.custom{border-color:rgba(169,140,255,.2);background:rgba(169,140,255,.045)}.mini-status-row{display:flex;justify-content:space-between;align-items:center;padding-top:11px;margin-top:11px;border-top:1px solid var(--line);font-size:7px;color:#718891}.mini-status-row strong{color:#92edf3}.health-row{display:flex;justify-content:space-between;gap:10px;padding:8px 0;border-bottom:1px solid rgba(27,43,51,.7);font-size:8px}.health-row span{color:#748a92}.health-row b{color:#dce8eb}.health-card .message{margin-top:11px}.runtime-card .data-list{margin-top:7px}.data-list{display:grid;gap:0}.data-list>div{display:flex;justify-content:space-between;gap:15px;padding:8px 0;border-bottom:1px solid rgba(27,43,51,.7)}.data-list span{color:#71868e;font-size:7px}.data-list b{font-size:8px;text-align:right}.data-list.compact b{max-width:65%;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.data-strip-card{grid-column:1/-1}.data-strip-title{display:flex;justify-content:space-between;align-items:center;gap:10px}.micro-grid{display:grid;grid-template-columns:repeat(8,minmax(0,1fr));gap:6px;margin-top:12px}.micro-grid>div{padding:9px 8px;background:#071015;border:1px solid var(--line);border-radius:9px;min-width:0}.micro-grid span{display:block;color:#607780;font-size:5px;text-transform:uppercase;letter-spacing:.11em}.micro-grid b{display:block;margin-top:5px;font-size:8px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}\n"
".play-layout{display:grid;grid-template-columns:minmax(0,1.18fr) minmax(320px,.62fr);gap:14px}.play-board-card{min-width:0}.play-frame{padding:14px 0}.side-stack{display:grid;gap:10px;align-content:start}.player-card{display:grid;grid-template-columns:auto 1fr auto;align-items:center;gap:9px;padding:10px 12px;background:#081015;border:1px solid var(--line);border-radius:12px}.player-card.cyan-side{border-color:rgba(39,216,233,.19)}.player-card.gray-side{border-color:rgba(190,200,205,.15)}.player-swatch{width:10px;height:28px;border-radius:5px;background:var(--cyan);box-shadow:0 0 13px rgba(39,216,233,.2)}.gray-side .player-swatch{background:#9ba7ac;box-shadow:none}.player-card small{display:block;color:#657a82;font-size:5px;letter-spacing:.12em}.player-card b{display:block;margin-top:4px;font-size:10px}.player-card strong{font-size:19px;letter-spacing:-.03em}.control-grid{display:grid;grid-template-columns:1fr 1fr;gap:7px;margin-top:11px}.toggle-row{display:grid;grid-template-columns:1fr auto 34px;align-items:center;gap:9px;margin-top:12px;font-size:8px;color:#7b9098}.toggle-row input{display:none}.toggle-pill{width:34px;height:19px;border-radius:999px;background:#1b2b31;border:1px solid #294149;position:relative}.toggle-pill:after{content:\"\";position:absolute;width:13px;height:13px;top:2px;left:2px;border-radius:50%;background:#7a8d94;transition:.18s}.toggle-row input:checked+.toggle-pill{background:rgba(39,216,233,.18);border-color:rgba(39,216,233,.45)}.toggle-row input:checked+.toggle-pill:after{left:17px;background:var(--cyan);box-shadow:0 0 11px rgba(39,216,233,.35)}.status-line{display:flex;justify-content:space-between;padding-top:9px;margin-top:9px;border-top:1px solid var(--line);font-size:8px}.status-line span{color:#71878f}.history{display:grid;gap:5px;max-height:260px;overflow:auto;margin-top:9px}.moveRow,.listItem{display:grid;grid-template-columns:28px 1fr 1fr;gap:7px;padding:7px 8px;background:#071015;border:1px solid var(--line);border-radius:8px;font-size:8px}.moveRow.current{border-color:rgba(39,216,233,.25)}.moveRow span{color:#82969d}.replay-row{display:flex;gap:6px;margin-top:8px}.replay-row button{flex:1}.fen{margin-top:9px;padding:10px;background:#060d11;border:1px solid var(--line);border-radius:9px;font:8px/1.45 ui-monospace,monospace;color:#9cb0b7;word-break:break-all}.captured-grid{display:grid;grid-template-columns:1fr 1fr;gap:7px;margin-top:8px}.captured-grid>div{padding:8px;background:#071015;border:1px solid var(--line);border-radius:8px}.captured-grid span{display:block;color:#627881;font-size:6px}.captured-grid b{display:block;margin-top:5px;font-size:13px;letter-spacing:.05em}\n"
".light-layout{display:grid;grid-template-columns:minmax(0,1.1fr) minmax(360px,.9fr);gap:14px}.led-main-card{min-width:0}.led-frame{display:grid;place-items:center;padding:16px 0 8px}.led-board{width:min(100%,700px);height:auto;aspect-ratio:1 / 1;display:grid;grid-template-columns:repeat(8,minmax(0,1fr));grid-template-rows:repeat(8,minmax(0,1fr));grid-auto-flow:row;gap:2px;padding:7px;border:1px solid #1e3139;border-radius:15px;background:#030709;box-shadow:inset 0 0 50px rgba(0,0,0,.55),0 24px 60px rgba(0,0,0,.36)}.led-cell{width:100%;height:100%;min-width:0;min-height:0;aspect-ratio:1 / 1 !important;border-radius:5px;background:#05090c;border:1px solid rgba(255,255,255,.045);cursor:pointer;transition:.1s;box-shadow:inset 0 0 8px rgba(0,0,0,.4)}.led-cell:hover{outline:2px solid rgba(39,216,233,.8);outline-offset:-2px;transform:scale(.96)}.led-axis{display:grid;grid-template-columns:repeat(5,1fr);align-items:center;color:#647a83;font-size:6px;letter-spacing:.08em}.led-axis span:nth-child(3){text-align:center;color:#8ea8b0}.led-axis span:nth-child(4),.led-axis span:nth-child(5){text-align:right}.led-toolbar{display:flex;gap:7px;flex-wrap:wrap;align-items:center;margin-top:13px}.inline-color{display:flex;align-items:center;gap:7px;padding:7px 9px;border:1px solid var(--line2);border-radius:9px;background:#081015;color:#70868e;font-size:7px;font-weight:800}.inline-color input{width:28px;height:23px;border:0;padding:0;background:transparent}.slider-grid{display:grid;grid-template-columns:1fr 1fr;gap:12px;margin-top:12px}.slider-grid label{display:grid;gap:7px;color:#728890;font-size:7px}.slider-grid input{width:100%;accent-color:var(--cyan)}.light-side{display:grid;gap:12px;align-content:start}.animation-card .section-head h3{margin-top:5px}.anim-grid{display:grid;grid-template-columns:1fr 1fr;gap:7px;margin-top:12px}.anim-btn{min-height:61px;text-align:left;padding:10px 11px}.anim-btn span{display:block;font-size:8px}.anim-btn small{display:block;margin-top:4px;color:#6b8189;font-size:6px;line-height:1.45}.anim-btn.active{border-color:rgba(39,216,233,.46);background:rgba(39,216,233,.075);box-shadow:0 0 0 1px rgba(39,216,233,.08),0 9px 24px rgba(39,216,233,.05)}.anim-btn.rainbow{background:linear-gradient(110deg,rgba(255,93,104,.16),rgba(242,200,91,.11),rgba(75,227,164,.1),rgba(39,216,233,.11),rgba(169,140,255,.13))}.anim-btn.custom.active{border-color:rgba(169,140,255,.5);background:rgba(169,140,255,.08)}.color-grid{display:grid;grid-template-columns:1fr 1fr;gap:7px;margin-top:11px}.color-item{display:grid;grid-template-columns:1fr 35px;align-items:center;gap:8px;padding:8px;background:#071015;border:1px solid var(--line);border-radius:8px}.color-item span{font-size:7px;color:#789099}.color-item input,.color-setting input,.custom-color input{width:35px;height:27px;border:0;padding:0;background:transparent}.color-setting{display:flex;justify-content:space-between;align-items:center;gap:10px;padding:8px 0;border-bottom:1px solid rgba(27,43,51,.7);font-size:8px;color:#8aa0a8}.custom-lab{margin-top:14px}.custom-controls{display:flex;gap:7px;flex-wrap:wrap;align-items:end;margin-top:12px}.custom-controls label,.field{display:grid;gap:6px;color:#748a92;font-size:7px}.custom-controls select,.custom-controls input,.field input,.field select,.field textarea,.field-inline input{min-height:38px;border:1px solid var(--line2);background:#060d11;color:var(--text);border-radius:9px;padding:8px 9px;outline:none}.field input:focus,.field select:focus,.field-inline input:focus{border-color:rgba(39,216,233,.52);box-shadow:0 0 0 3px rgba(39,216,233,.07)}.custom-palette{display:flex;gap:8px;flex-wrap:wrap;margin-top:12px}.custom-color{display:grid;gap:4px;width:66px}.custom-color small{font-size:6px;color:#667d85}.paint-tools,.frame-tabs{display:flex;gap:6px;align-items:center;flex-wrap:wrap;margin-top:10px}.paint-swatch{width:34px;min-height:32px;padding:5px}.paint-swatch.active{outline:2px solid #fff;outline-offset:1px}.frame-tabs button.active{border-color:rgba(39,216,233,.45);background:rgba(39,216,233,.07)}.paint-stage{display:grid;place-items:center}.paint-grid{width:min(100%,560px);height:auto;aspect-ratio:1 / 1;display:grid;grid-template-columns:repeat(8,minmax(0,1fr));grid-template-rows:repeat(8,minmax(0,1fr));grid-auto-flow:row;border:6px solid #122028;border-radius:13px;overflow:hidden;margin:13px auto 9px;background:#04080b}.paint-cell{width:100%;height:100%;min-width:0;min-height:0;aspect-ratio:1 / 1 !important;border:1px solid rgba(255,255,255,.045);cursor:pointer}\n"
".tile-grid{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:12px}.tile,.lesson,.puzzle{background:linear-gradient(145deg,#0c161c,#070d11);border:1px solid var(--line);border-radius:13px;padding:14px}.tile h3,.lesson h4,.puzzle h4{margin:8px 0 6px;font-size:12px}.tile p,.lesson p,.puzzle p{margin:0 0 11px;color:#789098;font-size:8px;line-height:1.6;min-height:38px}.archive-grid,.analysis-grid,.connection-grid,.settings-grid,.telemetry-top-grid{display:grid;grid-template-columns:1fr 1fr;gap:14px}.pgn{width:100%;min-height:420px;margin-top:9px;resize:vertical;background:#060d11;border:1px solid var(--line);border-radius:10px;padding:11px;color:#b7c9ce;font:8px/1.6 ui-monospace,monospace}.analysis-list{display:grid;gap:7px;max-height:590px;overflow:auto}.analysis-item{background:#071015;border:1px solid var(--line);border-radius:9px;padding:9px;font-size:8px}.ai-actions{margin-top:10px}.engine-output{min-height:250px;max-height:520px;overflow:auto;white-space:pre-wrap;background:#050a0d;border:1px solid var(--line);border-radius:10px;padding:11px;color:#a9c0c7;font:8px/1.6 ui-monospace,monospace;margin-top:10px}.connect-hero{display:grid;grid-template-columns:minmax(0,1fr) 330px 260px;gap:14px;align-items:end;margin-bottom:14px}.connect-hero h3{margin:6px 0}.connect-hero p{margin:0}.connect-hero-form{display:grid;grid-template-columns:1fr auto;gap:8px;align-items:end}.connect-hero-form .field{margin-top:0}.connection-card{min-width:0}.connection-icon{width:46px;height:46px;display:grid;place-items:center;border-radius:13px;border:1px solid rgba(39,216,233,.24);background:rgba(39,216,233,.04);color:#baf8fc;font-weight:900;font-size:13px;margin-bottom:11px}.connection-icon.chesscom{border-color:rgba(242,200,91,.24);background:rgba(242,200,91,.04);color:#f0d58a}.connection-icon.net{font-size:26px}.connection-icon.board{font-size:9px}.connection-card h3{margin:6px 0;font-size:13px}.connection-card>p{margin:0;color:#7b929b;font-size:8px;line-height:1.65}.connect-state-row{display:flex;gap:7px;flex-wrap:wrap;align-items:center;margin-top:11px}.button-link{text-decoration:none;display:inline-flex;align-items:center;justify-content:center}.field{margin-top:11px}.field span{color:#778d95}.connection-data{margin-top:10px;padding:10px;background:#050b0e;border:1px solid var(--line);border-radius:9px;white-space:pre-wrap;overflow:auto;max-height:260px;font:7px/1.6 ui-monospace,monospace;color:#9fb5bd}.official-note{margin-top:10px;padding-top:9px;border-top:1px solid var(--line);font-size:7px;line-height:1.7;color:#748a93}.setup-steps{display:grid;gap:5px;margin-top:11px;padding:9px;background:#071015;border:1px solid var(--line);border-radius:9px;color:#7d939b;font-size:7px}.setup-steps b{color:#b4cdd2}.setup-steps code{padding:2px 4px;border-radius:4px;background:#0d1a20;color:#b5edf1}.field-inline{display:grid;grid-template-columns:1fr 1fr;gap:7px;margin-top:10px}.field-inline label{display:grid;gap:6px;color:#778d95;font-size:7px}.profile-hero{display:grid;grid-template-columns:220px 1fr;gap:18px;align-items:start}.profile-hero h3{font-size:22px;margin:8px 0 0}.profile-stat-grid{display:grid;grid-template-columns:repeat(4,minmax(0,1fr));gap:8px}.stat{padding:11px;background:#071015;border:1px solid var(--line);border-radius:10px}.stat b{display:block;font-size:18px}.stat span{display:block;margin-top:3px;color:#6d838b;font-size:6px;letter-spacing:.08em;text-transform:uppercase}.check{display:flex;align-items:center;gap:8px;margin-top:11px;color:#82979f;font-size:8px}.check input{accent-color:var(--cyan)}.telemetry-top-grid{grid-template-columns:1fr 1fr}.telemetry-card{min-width:0}.telemetry-pre{margin-top:10px;min-height:230px;max-height:400px;overflow:auto;background:#04090c;border:1px solid var(--line);border-radius:10px;padding:11px;color:#a5bcc4;font:7px/1.65 ui-monospace,monospace;white-space:pre-wrap}.telemetry-pre.raw{min-height:430px}.telemetry-map{width:100%;max-width:280px;aspect-ratio:1;margin:10px auto 0;display:grid;grid-template-columns:repeat(8,minmax(0,1fr));grid-template-rows:repeat(8,minmax(0,1fr));gap:2px;padding:5px;background:#03070a;border:1px solid var(--line);border-radius:10px}.telemetry-cell{min-width:0;min-height:0;width:100%;height:100%;aspect-ratio:1;border-radius:3px;background:#061015;border:1px solid rgba(255,255,255,.045);position:relative}.telemetry-cell.active{background:rgba(39,216,233,.28);box-shadow:inset 0 0 0 2px rgba(144,247,255,.42),0 0 11px rgba(39,216,233,.18)}.telemetry-cell span{position:absolute;right:2px;bottom:1px;font-size:4px;color:rgba(255,255,255,.4)}.led-map-mini .telemetry-cell{background:#05090c}.full-span{grid-column:1/-1}.toast{position:fixed;right:18px;bottom:18px;z-index:100;max-width:min(460px,calc(100vw - 36px));padding:11px 13px;border:1px solid var(--line2);border-radius:10px;background:#0a1419;box-shadow:var(--shadow);font-size:8px;opacity:0;transform:translateY(8px);pointer-events:none;transition:.18s}.toast.show{opacity:1;transform:none}.toast.success{border-color:rgba(75,227,164,.28)}.toast.error{border-color:rgba(255,93,104,.3)}.toast.warning{border-color:rgba(242,200,91,.25)}\n"
"@media(max-width:1260px){.app-shell{grid-template-columns:228px minmax(0,1fr)}.main{padding:0 18px 24px}.command-hero{grid-template-columns:1fr}.hero-console{min-height:240px}.hero-copy{padding-bottom:10px}.cockpit-grid{grid-template-columns:1fr}.cockpit-board{grid-row:auto}.system-card{order:2}.quick-card{order:3}.health-card{order:4}.runtime-card{order:5}.data-strip-card{order:6}.micro-grid{grid-template-columns:repeat(4,1fr)}.play-layout,.light-layout{grid-template-columns:1fr}.light-side{grid-template-columns:1fr 1fr}.custom-lab{grid-column:1/-1}.connection-grid,.archive-grid,.analysis-grid,.settings-grid,.profile-hero{grid-template-columns:1fr}.connect-hero{grid-template-columns:1fr}.profile-stat-grid{grid-template-columns:repeat(4,1fr)}}\n"
"@media(max-width:900px){.sidebar{position:fixed;left:0;top:0;bottom:0;width:272px;transform:translateX(-102%);transition:.22s}.sidebar.open{transform:translateX(0)}.app-shell{grid-template-columns:1fr}.main{padding:0 13px 20px}.mobile-menu{display:block}.topbar{grid-template-columns:auto 1fr auto;padding-top:11px}.breadcrumb{display:none}.topbar-title{grid-column:2}.topbar-right{grid-column:3}.status-pill.optional{display:none}.page-headline{align-items:flex-start;flex-direction:column}.page-headline h2{font-size:26px}.hero-copy h2{font-size:38px}.light-side{grid-template-columns:1fr}.tile-grid{grid-template-columns:1fr 1fr}.telemetry-top-grid{grid-template-columns:1fr}}\n"
"@media(max-width:600px){.main{padding:0 10px 18px}.topbar h1{font-size:17px}.command-hero{min-height:0}.hero-copy{padding:24px}.hero-copy h2{font-size:32px}.hero-actions{display:grid;grid-template-columns:1fr}.hero-console{min-height:250px}.ring-a{width:300px;height:300px}.ring-b{width:225px;height:225px}.ring-c{width:150px;height:150px}.console-tag{display:none}.cockpit-grid{gap:10px}.board-frame{padding:11px 0 7px}.chess-board{width:100%;height:auto;border-width:4px;border-radius:10px}.sq{font-size:clamp(20px,9.2vw,42px)}.coord{font-size:5px}.micro-grid{grid-template-columns:1fr 1fr}.metric-grid.prime{grid-template-columns:1fr 1fr}.control-grid{grid-template-columns:1fr}.led-toolbar{display:grid;grid-template-columns:1fr 1fr}.led-toolbar>*{width:100%}.slider-grid{grid-template-columns:1fr}.anim-grid,.color-grid{grid-template-columns:1fr}.custom-controls{display:grid;grid-template-columns:1fr 1fr}.custom-controls label:nth-child(3){grid-column:1/-1}.custom-controls button:last-child{grid-column:1/-1}.tile-grid,.profile-stat-grid,.captured-grid,.field-inline{grid-template-columns:1fr}.connect-hero-form{grid-template-columns:1fr}.connect-state-row{display:grid;grid-template-columns:1fr}.topbar-right .status-pill{display:none}.status-pill.optional{display:none}.hardware-link{display:none}}\n"
"\n"
"\n"
"/* V11 visual hardening */\n"
"button,.button-link,.control-grid button,.lesson button,.puzzle button,.replay-row button{background:#0b171d !important;color:#eaf8fb !important;border:1px solid #29424d !important;box-shadow:0 4px 16px rgba(0,0,0,.16)}\n"
"button:hover,.button-link:hover,.control-grid button:hover,.lesson button:hover,.puzzle button:hover,.replay-row button:hover{background:#11252d !important;color:#ffffff !important;border-color:#3fc9d7 !important}\n"
"button.primary,.primary{background:linear-gradient(135deg,#26bfcf,#23dce9) !important;color:#031015 !important;border-color:#36e5ef !important}\n"
"button.secondary{background:#0b171d !important;color:#dff3f6 !important}\n"
".control-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:8px}.control-grid button{min-height:48px}\n"
".replay-row button{font-size:8px;letter-spacing:.08em}\n"
".position-readable{display:grid;gap:4px;margin-top:8px;font:700 8px/1.35 ui-monospace,monospace}\n"
".position-rank{display:grid;grid-template-columns:20px repeat(8,minmax(0,1fr));gap:3px;align-items:stretch}\n"
".position-rank b,.position-cell{padding:7px 4px;border:1px solid var(--line);border-radius:6px;background:#061015;text-align:center;min-width:0}\n"
".position-rank b{color:#718992;background:#071218}\n"
".position-cell.white-piece{color:#eaffff;border-color:rgba(39,216,233,.16)}.position-cell.black-piece{color:#c5d0d4;border-color:rgba(255,255,255,.08)}.position-cell.empty{color:#44575e}\n"
".raw-fen{margin-top:9px;padding:9px;background:#050b0e;border:1px solid var(--line);border-radius:9px;color:#71878f;font:7px/1.5 ui-monospace,monospace;overflow:auto}\n"
".captured-name{font:700 9px/1.5 ui-monospace,monospace;color:#d8eaee}\n"
".sq.legal,.sq.capture{box-shadow:inset 0 0 0 3px #f2c84b,inset 0 0 25px rgba(242,200,75,.36) !important}\n"
".sq.legal:after,.sq.capture:after{display:none !important}\n"
".sq.lifted{box-shadow:inset 0 0 0 4px #4aa9ff,inset 0 0 28px rgba(74,169,255,.38) !important}\n"
".sq.check{background:rgba(170,25,35,.42) !important;box-shadow:inset 0 0 0 4px #ff5d68,inset 0 0 32px rgba(255,60,70,.4) !important}\n"
".sq.selected{box-shadow:inset 0 0 0 3px var(--cyan),inset 0 0 22px rgba(39,216,233,.35)}\n"
".piece{filter:drop-shadow(0 3px 2px rgba(0,0,0,.42)) drop-shadow(0 0 10px currentColor)}\n"
".custom-lab input[type=text],.custom-lab input[type=number],.custom-lab select{min-height:38px}\n"
".custom-name-row{display:grid;grid-template-columns:minmax(150px,1fr) minmax(130px,1fr) auto auto auto;gap:8px;align-items:end;margin-top:10px}\n"
".custom-name-row label{display:grid;gap:5px;color:#789098;font-size:7px;letter-spacing:.08em}\n"
".paint-cell{min-width:0;aspect-ratio:1;border:1px solid #29424d;border-radius:5px;cursor:pointer;touch-action:manipulation;transition:.12s}.paint-cell:hover{outline:2px solid rgba(39,216,233,.38)}\n"
".live-ok{color:#72efb1 !important}.live-off{color:#ff7f88 !important}\n"
"@media(max-width:900px){.nav{max-height:none;flex:1;overflow-y:auto}.sidebar{overflow:hidden}.main{padding:0 10px 20px}.topbar{gap:8px}.topbar-right{gap:5px}.status-pill{padding:7px 7px}.custom-name-row{grid-template-columns:1fr 1fr}.control-grid{grid-template-columns:1fr}.position-rank{grid-template-columns:18px repeat(8,minmax(0,1fr));font-size:6px}.position-rank b,.position-cell{padding:5px 2px}}\n"
"@media(max-width:560px){.topbar-right .status-pill.optional{display:none}.topbar h1{font-size:17px}.page-headline h2{font-size:24px}.tile-grid{grid-template-columns:1fr}.custom-name-row{grid-template-columns:1fr}.play-layout{grid-template-columns:1fr !important}.position-rank{font-size:5px}.position-rank b,.position-cell{padding:4px 1px}}\n"
"body.theme-midnight{--bg:#02040a;--panel:#070b13;--panel2:#0a0f18;--line:#172331;--line2:#26394a;--cyan:#6ca8ff}body.theme-frost{--bg:#0a1014;--panel:#101920;--panel2:#152027;--line:#2c424c;--line2:#3c5863;--cyan:#7de7ee}body.lang-ar{direction:rtl}body.lang-ar .sidebar{border-right:0;border-left:1px solid var(--line)}body.lang-ar .nav-item{text-align:right}body.lang-ar .coord{right:auto;left:4px}\n"
"\n"
"/* V12 professional hardening */\n"
":root{--focus:#27d8e9}\n"
"button,.button-link{font-weight:800;letter-spacing:.025em}\n"
"button:focus-visible,.button-link:focus-visible,input:focus-visible,select:focus-visible,textarea:focus-visible{outline:2px solid var(--focus);outline-offset:2px}\n"
".sidebar{overflow:hidden}.nav{flex:1;min-height:0;overflow-y:auto;overscroll-behavior:contain}.nav-item{min-height:42px}\n"
".main{min-width:0;overflow-x:hidden}.card,.tile,.lesson,.puzzle{min-width:0}\n"
".position-mini-board{display:grid;grid-template-columns:repeat(8,minmax(0,1fr));aspect-ratio:1/1;width:min(100%,520px);margin:10px auto 12px;border:5px solid #122028;border-radius:12px;overflow:hidden;box-shadow:0 14px 34px rgba(0,0,0,.25)}\n"
".position-mini-cell{position:relative;display:grid;place-items:center;min-width:0;overflow:hidden}.position-mini-cell.light{background:#27353b}.position-mini-cell.dark{background:#121c21}.position-mini-cell small{position:absolute;left:3px;top:2px;font-size:6px;opacity:.46;color:#b8cdd3}.position-mini-cell.occupied{box-shadow:inset 0 0 0 2px rgba(75,227,164,.55),inset 0 0 18px rgba(75,227,164,.18)}\n"
".mini-piece{font-size:clamp(24px,5vw,48px);line-height:1;font-family:\"Segoe UI Symbol\",\"Noto Sans Symbols 2\",\"DejaVu Sans\",serif;font-weight:700;filter:drop-shadow(0 3px 2px rgba(0,0,0,.55)) drop-shadow(0 0 7px rgba(255,255,255,.18));transform:translateY(-1px)}\n"
".mini-piece.white-piece{color:#f4ffff;text-shadow:0 1px 0 #fff,0 3px 0 rgba(0,0,0,.42),0 6px 10px rgba(0,0,0,.5)}.mini-piece.black-piece{color:#aab7bb;text-shadow:0 1px 0 rgba(255,255,255,.22),0 3px 0 rgba(0,0,0,.65),0 6px 10px rgba(0,0,0,.7)}\n"
".position-inventory{border-top:1px solid var(--line);padding-top:9px}.inventory-title{font-size:7px;letter-spacing:.16em;color:#708891;font-weight:900;margin-bottom:7px}.inventory-items{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:4px}.inventory-items span{background:#061015;border:1px solid var(--line);border-radius:7px;padding:5px 6px;color:#b9ccd1;font-size:7px;line-height:1.35}\n"
".tile-kicker{color:#6e8b94;font-size:6px;letter-spacing:.16em;font-weight:900;margin-bottom:6px}.tile-detail{margin:-2px 0 12px;padding:8px 9px;background:#061015;border:1px solid var(--line);border-radius:8px;color:#8fa8b0;font-size:7px;line-height:1.55;min-height:46px}.lesson,.puzzle{display:flex;flex-direction:column}.lesson button,.puzzle button{width:100%;margin-top:auto}\n"
".diagnostic-details{margin-top:8px}.diagnostic-details summary{cursor:pointer;color:#8fdfe5;font-size:8px;font-weight:800}.diagnostic-details .raw{margin-top:8px}\n"
"@media(max-width:900px){.sidebar{overflow:hidden}.nav{flex:1;min-height:0;overflow-y:auto}.topbar-title h1{max-width:42vw;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.play-layout{grid-template-columns:minmax(0,1fr)}.position-mini-board{width:min(100%,560px)}}\n"
"@media(max-width:600px){.sidebar{width:min(88vw,300px)}.nav-item{min-height:46px}.topbar{padding-left:8px;padding-right:8px}.mobile-menu{display:grid;place-items:center;font-size:20px}.topbar-right{gap:4px}.icon-button{width:40px;height:40px}.page-headline h2{font-size:25px}.page-headline p{font-size:9px}.inventory-items{grid-template-columns:1fr}.tile-detail{font-size:8px}}\n"
"</style>\n"
"</head>\n"
"<body>\n"
"<div class=\"app-shell\">\n"
"  <aside class=\"sidebar\" id=\"sidebar\">\n"
"    <div class=\"brand-block\">\n"
"      <div class=\"brand-symbol\"><span></span><span></span><span></span><span></span></div>\n"
"      <div><div class=\"brand-name\">SMART CHESS</div><div class=\"brand-meta\">PHYSICAL CHESS OS</div></div>\n"
"    </div>\n"
"    <div class=\"version-mark\">V12 // PRIME CONTROL</div>\n"
"\n"
"    <div class=\"side-live\">\n"
"      <i id=\"sideDot\" class=\"dot\"></i>\n"
"      <div><b id=\"sideBoardText\">Board offline</b><span>Live hardware link</span></div>\n"
"    </div>\n"
"\n"
"    <nav class=\"nav\" aria-label=\"Primary\">\n"
"      <button class=\"nav-item active\" data-page=\"home\"><span class=\"nav-index\">01</span><span>Command Deck</span></button>\n"
"      <button class=\"nav-item\" data-page=\"play\"><span class=\"nav-index\">02</span><span>Play Chess</span></button>\n"
"      <button class=\"nav-item\" data-page=\"led\"><span class=\"nav-index\">03</span><span>Light Lab</span></button>\n"
"      <button class=\"nav-item\" data-page=\"learn\"><span class=\"nav-index\">04</span><span>Learn</span></button>\n"
"      <button class=\"nav-item\" data-page=\"puzzles\"><span class=\"nav-index\">05</span><span>Puzzles</span></button>\n"
"      <button class=\"nav-item\" data-page=\"games\"><span class=\"nav-index\">06</span><span>Game Archive</span></button>\n"
"      <button class=\"nav-item\" data-page=\"analysis\"><span class=\"nav-index\">07</span><span>Analysis</span></button>\n"
"      <button class=\"nav-item\" data-page=\"connect\"><span class=\"nav-index\">08</span><span>Connections</span></button>\n"
"      <button class=\"nav-item\" data-page=\"profile\"><span class=\"nav-index\">09</span><span>Profile</span></button>\n"
"      <button class=\"nav-item\" data-page=\"settings\"><span class=\"nav-index\">10</span><span>Settings</span></button>\n"
"      <button class=\"nav-item\" data-page=\"telemetry\"><span class=\"nav-index\">11</span><span>Live Diagnostics</span></button>\n"
"    </nav>\n"
"\n"
"    <div class=\"sidebar-footer\">\n"
"      <div class=\"hardware-badge\"><b>ESP32-WROOM-32U</b><span>8×8 MATRIX • 64 RGB • GPIO 2 DATA</span></div>\n"
"      <div class=\"side-footline\"><span>H8</span><b>ACTIVE</b><span>•</span><span id=\"liveText\">BOARD OFFLINE</span></div>\n"
"    </div>\n"
"  </aside>\n"
"\n"
"  <main class=\"main\">\n"
"    <header class=\"topbar\">\n"
"      <button class=\"mobile-menu\" id=\"mobileMenu\" aria-label=\"Menu\">≡</button>\n"
"      <div class=\"breadcrumb\" id=\"crumb\">SMART CHESS / COMMAND DECK</div>\n"
"      <div class=\"topbar-title\"><h1 id=\"pageTitle\">Command Deck</h1><span class=\"tiny-version\">V12</span></div>\n"
"      <div class=\"topbar-right\">\n"
"        <div class=\"status-pill\"><i id=\"liveDot\" class=\"dot\"></i><span>HARDWARE</span><b id=\"topBoardText\">OFFLINE</b></div>\n"
"        <div class=\"status-pill optional\"><i class=\"dot idle\"></i><span>AI</span><b id=\"topAiText\">NOT SET</b></div>\n"
"        <button class=\"icon-button\" id=\"refreshBtn\" title=\"Refresh live data\">↻</button>\n"
"      </div>\n"
"    </header>\n"
"\n"
"    <section id=\"page-home\" class=\"page active\">\n"
"      <div class=\"command-hero\">\n"
"        <div class=\"hero-copy\">\n"
"          <div class=\"eyebrow accent-text\">V10 / PHYSICAL INTELLIGENCE</div>\n"
"          <h2>The board is the interface.</h2>\n"
"          <p>One control center for the physical board, all 64 LEDs, animations, chess state, hardware telemetry, AI and online connectors.</p>\n"
"          <div class=\"hero-actions\">\n"
"            <button id=\"heroPlay\" class=\"primary\">Open Chess</button>\n"
"            <button id=\"heroLed\" class=\"secondary\">Open Light Lab</button>\n"
"            <button id=\"heroTelemetry\" class=\"ghost\">Inspect Diagnostics</button>\n"
"          </div>\n"
"        </div>\n"
"        <div class=\"hero-console\">\n"
"          <div class=\"console-ring ring-a\"></div><div class=\"console-ring ring-b\"></div><div class=\"console-ring ring-c\"></div>\n"
"          <div class=\"console-core\"><strong>8×8</strong><span>64 CHANNELS</span><em>V10</em></div>\n"
"          <div class=\"console-tag tag-a\">SENSOR ARRAY</div><div class=\"console-tag tag-b\">LED ENGINE</div><div class=\"console-tag tag-c\">RULE ENGINE</div>\n"
"        </div>\n"
"      </div>\n"
"\n"
"      <div class=\"cockpit-grid\">\n"
"        <div class=\"card board-card cockpit-board\">\n"
"          <div class=\"section-head\">\n"
"            <div><div class=\"eyebrow\">PHYSICAL BOARD MIRROR</div><h3>Live 8×8 position</h3></div>\n"
"            <div class=\"head-chips\"><span class=\"chip accent\">A1 → H8</span><span class=\"chip\" id=\"homeStateChip\">READY</span></div>\n"
"          </div>\n"
"          <div class=\"board-frame\"><div id=\"homeBoard\" class=\"chess-board board-v10\" data-board=\"physical\" aria-label=\"Smart Chess physical board\"></div></div>\n"
"          <div class=\"board-legend\"><span><i class=\"legend-dot cyan\"></i>Cyan side</span><span><i class=\"legend-dot gray\"></i>Gray side</span><span><i class=\"legend-dot guide\"></i>Guide</span><span><i class=\"legend-dot warn\"></i>Warning</span><span id=\"homeBoardStatus\">Waiting for telemetry</span></div>\n"
"        </div>\n"
"\n"
"        <div class=\"card system-card\">\n"
"          <div class=\"section-head\"><div><div class=\"eyebrow\">SYSTEM SNAPSHOT</div><h3>Board state</h3></div><span class=\"signal\" id=\"homeSignal\">LINK —</span></div>\n"
"          <div class=\"metric-grid prime\">\n"
"            <div class=\"metric hot\"><span>Connection</span><b id=\"mBoard\">OFFLINE</b></div>\n"
"            <div class=\"metric\"><span>Turn</span><b id=\"mTurn\">Cyan</b></div>\n"
"            <div class=\"metric\"><span>Chess</span><b id=\"mState\">Ready</b></div>\n"
"            <div class=\"metric\"><span>Pieces / sensors</span><b id=\"mActive\">0</b></div>\n"
"            <div class=\"metric\"><span>Animation</span><b id=\"mAnim\">OFF</b></div>\n"
"            <div class=\"metric\"><span>Brightness</span><b id=\"mBright\">80</b></div>\n"
"            <div class=\"metric\"><span>Free heap</span><b id=\"mHeap\">—</b></div>\n"
"            <div class=\"metric\"><span>CPU</span><b id=\"mCpu\">—</b></div>\n"
"          </div>\n"
"          <div class=\"event-panel\"><div class=\"event-kicker\">LATEST HARDWARE EVENT</div><div class=\"event-main\" id=\"latestEvent\">Waiting for switch activity</div><div class=\"event-square\" id=\"latestSquare\">--</div></div>\n"
"          <div id=\"homeMessage\" class=\"message info\">Waiting for Smart Chess board telemetry.</div>\n"
"        </div>\n"
"\n"
"        <div class=\"card quick-card\">\n"
"          <div class=\"section-head\"><div><div class=\"eyebrow\">QUICK LIGHT COMMANDS</div><h3>One tap control</h3></div></div>\n"
"          <div class=\"quick-grid\">\n"
"            <button id=\"qOff\" class=\"quick danger-soft\"><b>ALL OFF</b><span>Clear LEDs</span></button>\n"
"            <button id=\"qOn\" class=\"quick\"><b>ALL ON</b><span>White test</span></button>\n"
"            <button id=\"qMatrix\" class=\"quick\"><b>SENSOR MAP</b><span>Show active squares</span></button>\n"
"            <button id=\"qCenter\" class=\"quick\"><b>CENTER OUT</b><span>Run animation</span></button>\n"
"            <button id=\"qRainbow\" class=\"quick rainbow\"><b>RAINBOW</b><span>6-color flow</span></button>\n"
"            <button id=\"qCustom\" class=\"quick custom\"><b>CUSTOM</b><span>Your saved animation</span></button>\n"
"          </div>\n"
"          <div class=\"mini-status-row\"><span>Animation control</span><strong id=\"homeAnimState\">OFF</strong></div>\n"
"        </div>\n"
"\n"
"        <div class=\"card health-card\">\n"
"          <div class=\"section-head\"><div><div class=\"eyebrow\">MATRIX HEALTH</div><h3>Signal monitor</h3></div></div>\n"
"          <div class=\"health-row\"><span>Raw active cells</span><b id=\"mRaw\">0</b></div>\n"
"          <div class=\"health-row\"><span>Raw row mask</span><b id=\"rawRowMask\">0</b></div>\n"
"          <div class=\"health-row\"><span>Raw column mask</span><b id=\"rawColMask\">0</b></div>\n"
"          <div class=\"health-row\"><span>Multiple detection</span><b id=\"multiRaw\">NO</b></div>\n"
"          <div id=\"diagMessage\" class=\"message success\">Matrix reading normally.</div>\n"
"        </div>\n"
"\n"
"        <div class=\"card runtime-card\">\n"
"          <div class=\"section-head\"><div><div class=\"eyebrow\">DEVICE STATUS</div><h3>Hardware facts</h3></div></div>\n"
"          <div class=\"data-list compact\"><div><span>IP</span><b id=\"deviceIP\">—</b></div><div><span>Sensitivity</span><b id=\"setSensitivity\">—</b></div><div><span>Settle</span><b id=\"setSettle\">—</b></div><div><span>Calibration</span><b id=\"setCalibration\">0/64</b></div><div><span>Mapping</span><b id=\"setMapping\">—</b></div></div>\n"
"        </div>\n"
"\n"
"        <div class=\"card data-strip-card\">\n"
"          <div class=\"data-strip-title\"><div><div class=\"eyebrow\">LIVE DIAGNOSTICS</div><h3>Live board diagnostics</h3></div><button class=\"secondary small-btn\" data-page=\"telemetry\" onclick=\"document.querySelector('[data-page=\\'telemetry\\']')?.click()\">OPEN DIAGNOSTICS</button></div>\n"
"          <div class=\"micro-grid\"><div><span>Wi-Fi network</span><b id=\"homeSSID\">—</b></div><div><span>Connection mode</span><b id=\"homeWifi\">—</b></div><div><span>Uptime</span><b id=\"homeUptime\">—</b></div><div><span>Raw sensor cells</span><b id=\"homeRawCells\">—</b></div><div><span>LED mode</span><b id=\"homeLEDMode\">—</b></div><div><span>Game</span><b id=\"homeGameMode\">—</b></div><div><span>Game score</span><b id=\"homeGameScore\">0</b></div><div><span>Moves</span><b id=\"homeGameMoves\">0</b></div></div>\n"
"        </div>\n"
"      </div>\n"
"    </section>\n"
"\n"
"    <section id=\"page-play\" class=\"page\">\n"
"      <div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">PLAY / PHYSICAL + DIGITAL</div><h2>Real chess, with the board doing the talking.</h2><p>Legal moves, king safety, move history, replay and physical-board synchronization.</p></div><div class=\"page-actions\"><span class=\"chip accent\" id=\"playModeChip\">LOCAL</span><button class=\"secondary small-btn\" id=\"startChess\">START</button><button class=\"small-btn\" id=\"resetChess\">RESET</button></div></div>\n"
"      <div class=\"play-layout\">\n"
"        <div class=\"card board-card play-board-card\">\n"
"          <div class=\"section-head\"><div><div class=\"eyebrow\">LIVE POSITION</div><h3>Chessboard</h3></div><span class=\"chip\" id=\"physicalBoardState\">SYNC</span></div>\n"
"          <div class=\"board-frame play-frame\"><div id=\"chessBoard\" class=\"chess-board board-v10\" data-board=\"play\" aria-label=\"Smart Chess game board\"></div></div>\n"
"          <div id=\"physicalMessage\" class=\"message info\">Select a piece or lift it physically to show legal destinations.</div>\n"
"        </div>\n"
"        <div class=\"side-stack\">\n"
"          <div class=\"player-card cyan-side\"><span class=\"player-swatch\"></span><div><small>WHITE / CYAN</small><b id=\"playWhiteName\">Cyan</b></div><strong id=\"clockCyan\">10:00</strong></div>\n"
"          <div class=\"player-card gray-side\"><span class=\"player-swatch\"></span><div><small>BLACK / GRAY</small><b id=\"playBlackName\">Gray</b></div><strong id=\"clockGray\">10:00</strong></div>\n"
"          <div class=\"card match-card\"><div class=\"section-head\"><div><div class=\"eyebrow\">MATCH CONTROL</div><h3>Match controls</h3></div></div><div class=\"control-grid\"><button id=\"startGame\" class=\"primary\">START LOCAL 2-PLAYER</button><button id=\"resetGame\">RESET BOARD POSITION</button><button id=\"hintChess\">SHOW LEGAL DESTINATIONS</button><button id=\"offChess\">CLEAR BOARD LIGHTS</button></div><label class=\"toggle-row\"><span>Timed chess</span><input id=\"enableTimer\" type=\"checkbox\"><span class=\"toggle-pill\"></span></label><div class=\"status-line\"><span>Current turn</span><b id=\"playTurn\">Cyan</b></div><div class=\"status-line\"><span>Result</span><b id=\"playResult\">playing</b></div><div class=\"status-line\"><span>Halfmove clock</span><b id=\"playHalf\">0</b></div></div>\n"
"          <div class=\"card history-card\"><div class=\"section-head\"><div><div class=\"eyebrow\">MOVE HISTORY</div><h3>Game record</h3></div></div><div id=\"playHistory\" class=\"history\"><div class=\"muted small\">No moves recorded yet.</div></div><div class=\"replay-row\"><button id=\"replayStart\" title=\"Replay from start\">START</button><button id=\"replayBack\" title=\"Previous move\">BACK</button><button id=\"replayForward\" title=\"Next move\">NEXT</button><button id=\"replayEnd\" title=\"Replay to end\">END</button></div></div>\n"
"          <div class=\"card position-card\"><div class=\"eyebrow\">CURRENT POSITION</div><h3>Board map & piece inventory</h3><div id=\"playFen\" class=\"position-readable\"></div><details class=\"diagnostic-details\"><summary>Technical position data</summary><div id=\"playFenRaw\" class=\"raw-fen\"></div></details><div class=\"captured-grid\"><div><span>Captured by Cyan</span><b id=\"capturedCyan\" class=\"captured-name\">None</b></div><div><span>Captured by Gray</span><b id=\"capturedGray\" class=\"captured-name\">None</b></div></div></div>\n"
"        </div>\n"
"      </div>\n"
"    </section>\n"
"\n"
"    <section id=\"page-led\" class=\"page\">\n"
"      <div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">LIGHT LAB / 64-CHANNEL LED ENGINE</div><h2>Command every square.</h2><p>Control individual LEDs, run animations, edit palettes, and build a custom sequence without leaving the Smart Chess app.</p></div><span class=\"chip accent\">64 / 64 ADDRESSABLE</span></div>\n"
"      <div class=\"light-layout\">\n"
"        <div class=\"card led-main-card\">\n"
"          <div class=\"section-head\"><div><div class=\"eyebrow\">LIVE LED MATRIX</div><h3>Physical light state</h3></div><span class=\"chip\" id=\"ledModeChip\">OFF</span></div>\n"
"          <div class=\"led-frame\"><div id=\"ledBoard\" class=\"led-board board-v10\" data-board=\"led\" aria-label=\"Smart Chess LED map\"></div></div>\n"
"          <div class=\"led-axis\"><span>A1</span><span>LED 01</span><span id=\"ledCenterStatus\">Live mirror</span><span>LED 64</span><span>H8</span></div>\n"
"          <div class=\"led-toolbar\"><button id=\"ledOff\" class=\"secondary\">ALL OFF</button><button id=\"ledOn\" class=\"primary\">ALL ON</button><button id=\"ledMatrix\" class=\"secondary\">SENSOR MAP</button><label class=\"inline-color\">MANUAL <input id=\"manualColor\" type=\"color\" value=\"#27D8E9\"></label></div>\n"
"          <div class=\"slider-grid\"><label><span>Brightness <b id=\"brightLabel\">80</b></span><input id=\"brightRange\" type=\"range\" min=\"1\" max=\"255\" value=\"80\"></label><label><span>Animation speed <b id=\"speedLabel\">100</b></span><input id=\"speedRange\" type=\"range\" min=\"10\" max=\"255\" value=\"100\"></label></div>\n"
"          <div class=\"message info\">Click a square to command that LED directly. The preview uses the same A1-H8 coordinate system as the physical board.</div>\n"
"        </div>\n"
"\n"
"        <div class=\"light-side\">\n"
"          <div class=\"card animation-card\">\n"
"            <div class=\"section-head\"><div><div class=\"eyebrow\">ANIMATION CONTROL</div><h3>Run it from the app</h3></div><span class=\"chip\" id=\"animationStateChip\">OFF</span></div>\n"
"            <div class=\"anim-grid\">\n"
"              <button class=\"anim-btn rainbow\" data-anim=\"rainbow\"><span>RAINBOW</span><small>6 colors • smooth flow</small></button>\n"
"              <button class=\"anim-btn\" data-anim=\"wave\"><span>WAVE</span><small>2 colors • gradient</small></button>\n"
"              <button class=\"anim-btn\" data-anim=\"chase\"><span>CHASE</span><small>3 colors • moving trail</small></button>\n"
"              <button class=\"anim-btn\" data-anim=\"scan\"><span>SCAN</span><small>3 colors • sweep</small></button>\n"
"              <button class=\"anim-btn\" data-anim=\"breath\"><span>BREATH</span><small>1 color • pulse</small></button>\n"
"              <button class=\"anim-btn\" data-anim=\"random\"><span>RANDOM</span><small>4 colors • randomized field</small></button>\n"
"              <button class=\"anim-btn\" data-anim=\"centerout\"><span>CENTER OUT</span><small>2 colors • expanding rings</small></button>\n"
"              <button class=\"anim-btn custom\" data-anim=\"custom\"><span>CUSTOM</span><small>Saved frames from your lab</small></button>\n"
"              <button class=\"anim-btn stop\" data-anim=\"none\"><span>STOP</span><small>Return control to tracking</small></button>\n"
"            </div>\n"
"          </div>\n"
"\n"
"          <div class=\"card palette-card\">\n"
"            <div class=\"section-head\"><div><div class=\"eyebrow\">ACTIVE PALETTE</div><h3 id=\"paletteTitle\">Select an animation</h3></div><button class=\"primary small-btn\" id=\"savePalette\">SAVE</button></div>\n"
"            <p class=\"muted\" id=\"paletteHelp\">Choose an animation above to edit its exact color set.</p>\n"
"            <div id=\"paletteFields\" class=\"color-grid\"></div>\n"
"          </div>\n"
"\n"
"          <div class=\"card piece-color-card\">\n"
"            <div class=\"eyebrow\">BOARD COMMUNICATION COLORS</div><h3>Piece + move feedback</h3>\n"
"            <div class=\"color-setting\"><span>Piece on square</span><input type=\"color\" id=\"trackingColor\" value=\"#00FF50\"></div>\n"
"            <div class=\"color-setting\"><span>Player 1 / Cyan</span><input type=\"color\" id=\"p1Color\" value=\"#00D2FF\"></div>\n"
"            <div class=\"color-setting\"><span>Player 2 / Gray</span><input type=\"color\" id=\"p2Color\" value=\"#FF50B4\"></div>\n"
"            <div class=\"color-setting\"><span>Arcade / general guide</span><input type=\"color\" id=\"guideColor\" value=\"#005ABF\"></div>\n"
"            <div class=\"color-setting\"><span>Legal chess destination</span><input type=\"color\" id=\"legalColor\" value=\"#FFD230\"></div>\n"
"            <div class=\"color-setting\"><span>Lifted origin</span><input type=\"color\" id=\"liftColor\" value=\"#42AAFF\"></div>\n"
"            <div class=\"color-setting\"><span>Check / illegal destination</span><input type=\"color\" id=\"warningColor\" value=\"#FF0000\"></div>\n"
"            <button class=\"primary full\" id=\"savePieceColors\">SAVE COMMUNICATION COLORS</button>\n"
"          </div>\n"
"        </div>\n"
"      </div>\n"
"\n"
"      <div class=\"card custom-lab\">\n"
"        <div class=\"section-head\"><div><div class=\"eyebrow\">CUSTOM ANIMATION LAB</div><h3>Build named animations in seconds</h3><p>Choose a color, tap any board tile to paint it immediately, repeat a frame, name the animation, then save it to a slot in ESP32 flash. Up to 8 frames × 8 colors × 8 saved animations.</p></div><span class=\"chip\">USER DEFINED</span></div>\n"
"        <div class=\"custom-name-row\">\n"
"          <label>SAVED ANIMATION<select id=\"customSlot\"><option value=\"0\">Slot 1</option><option value=\"1\">Slot 2</option><option value=\"2\">Slot 3</option><option value=\"3\">Slot 4</option><option value=\"4\">Slot 5</option><option value=\"5\">Slot 6</option><option value=\"6\">Slot 7</option><option value=\"7\">Slot 8</option></select></label>\n"
"          <label>NAME<input id=\"customName\" type=\"text\" maxlength=\"18\" value=\"My Animation\" placeholder=\"My Animation\"></label>\n"
"          <button id=\"loadCustom\" class=\"secondary\">LOAD</button><button id=\"deleteCustom\" class=\"secondary\">DELETE</button><button id=\"saveCustom\" class=\"primary\">SAVE + PLAY</button>\n"
"        </div>\n"
"        <div class=\"custom-controls\"><label>Frames<select id=\"customFrames\"><option>1</option><option>2</option><option>3</option><option>4</option><option>5</option><option>6</option><option>7</option><option>8</option></select></label><label>Colors<select id=\"customColors\"><option>1</option><option>2</option><option>3</option><option selected>4</option><option>5</option><option>6</option><option>7</option><option>8</option></select></label><label>Frame delay (ms)<input id=\"customDelay\" type=\"number\" min=\"20\" max=\"2000\" value=\"120\"></label><label>Repeat time (sec)<input id=\"customRepeat\" type=\"number\" min=\"0\" max=\"600\" value=\"0\"></label><button id=\"clearFrame\">CLEAR FRAME</button><button id=\"repeatFrame\" class=\"secondary\">REPEAT FRAME</button></div>\n"
"        <div id=\"customPalette\" class=\"custom-palette\"></div>\n"
"        <div id=\"paintTools\" class=\"paint-tools\"></div>\n"
"        <div id=\"frameTabs\" class=\"frame-tabs\"></div>\n"
"        <div class=\"paint-stage\"><div id=\"customPaint\" class=\"paint-grid\"></div></div>\n"
"        <div id=\"customStatus\" class=\"message info\">Select a palette color, then tap board tiles. No add-color button is needed.</div>\n"
"      </div>\n"
"    </section>\n"
"\n"
"    <section id=\"page-learn\" class=\"page\"><div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">LEARN</div><h2>Interactive chess lessons with real positions.</h2><p>Open a lesson, load its starting position, and practice the objective on the board.</p></div></div><div id=\"lessonGrid\" class=\"tile-grid\"></div></section>\n"
"    <section id=\"page-puzzles\" class=\"page\"><div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">PUZZLES</div><h2>Practice verified training positions on the real board.</h2><p>Curated tactical positions with legal-move validation and physical LED feedback.</p><div class=\"message info\">Built-in positions are deterministic training content. Live tournament or AI-generated feeds belong in the external Hub.</div></div></div><div id=\"puzzleGrid\" class=\"tile-grid\"></div></section>\n"
"\n"
"    <section id=\"page-games\" class=\"page\"><div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">GAME ARCHIVE</div><h2>Every game, every move.</h2><p>Local saves, replay, rename, delete and PGN export.</p></div></div><div class=\"archive-grid\"><div class=\"card\"><div class=\"section-head\"><div><div class=\"eyebrow\">SAVED GAMES</div><h3>Archive</h3></div></div><div id=\"gamesList\" class=\"history tall\"></div></div><div class=\"card\"><div class=\"eyebrow\">PGN / STANDARD CHESS RECORD</div><h3>Import or export</h3><textarea id=\"pgnBox\" class=\"pgn\" placeholder=\"Paste or generate PGN here...\"></textarea><button class=\"primary full\" id=\"exportCurrent\">EXPORT CURRENT GAME</button></div></div></section>\n"
"\n"
"    <section id=\"page-analysis\" class=\"page\"><div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">ANALYSIS</div><h2>Explain the position, then call a real engine.</h2><p>Rule-based signals are always available. Stockfish output appears only when the real engine bridge is connected.</p></div><span class=\"chip warn\" id=\"aiBadge\">AI NOT CONNECTED</span></div><div class=\"analysis-grid\"><div class=\"card\"><div class=\"eyebrow\">RULE ENGINE</div><h3>Move-by-move explanation</h3><div id=\"analysisBox\" class=\"analysis-list\"></div></div><div class=\"card\"><div class=\"section-head\"><div><div class=\"eyebrow\">REAL ENGINE</div><h3>Stockfish</h3></div><button id=\"aiStatusBtn\" class=\"secondary\">TEST</button></div><div id=\"aiStatusBox\" class=\"message info\">Configure the Smart Chess Hub / Stockfish bridge.</div><div class=\"ai-actions\"><button id=\"aiAnalyzeBtn\" class=\"primary\">ANALYZE CURRENT POSITION</button></div><pre id=\"aiOutput\" class=\"engine-output\"></pre></div></div></section>\n"
"\n"
"    <section id=\"page-connect\" class=\"page\">\n"
"      <div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">CONNECTIONS / EXTERNAL SERVICES</div><h2>Connect Smart Chess to the outside world.</h2><p>Hardware stays local. The Smart Chess Hub provides the bridge to Stockfish, online rooms and official Chess.com integration points.</p></div><span class=\"chip accent\">NO PASSWORDS STORED</span></div>\n"
"      <div class=\"connect-hero card\">\n"
"        <div><div class=\"eyebrow\">SMART CHESS HUB</div><h3>One gateway for AI + online features</h3><p class=\"muted\">Run the included Node server on a computer. Put its LAN URL here, for example <b>http://192.168.1.50:8080</b>.</p></div>\n"
"        <div class=\"connect-hero-form\"><label class=\"field\"><span>Hub URL</span><input id=\"hubUrl\" placeholder=\"http://192.168.1.50:8080\"></label><button id=\"hubTest\" class=\"primary\">TEST HUB</button></div>\n"
"        <div id=\"hubState\" class=\"connection-data\">Not tested yet.</div>\n"
"      </div>\n"
"      <div class=\"connection-grid\">\n"
"        <div class=\"card connection-card ai-card\"><div class=\"connection-icon\">AI</div><div class=\"eyebrow\">REAL CHESS ENGINE</div><h3>Stockfish / UCI</h3><p>Smart Chess does not fake analysis. The Node server launches your locally installed Stockfish executable and returns real UCI output.</p><label class=\"field\"><span>AI / Hub URL</span><input id=\"aiUrl\" placeholder=\"http://192.168.1.50:8080\"></label><label class=\"field\"><span>Depth</span><select id=\"aiDepth\"><option value=\"8\">Beginner • depth 8</option><option value=\"12\">Easy • depth 12</option><option value=\"16\" selected>Medium • depth 16</option><option value=\"20\">Hard • depth 20</option><option value=\"24\">Expert • depth 24</option></select></label><div class=\"row\"><button id=\"saveAi\" class=\"primary\">SAVE + TEST</button><button id=\"aiStatus\" class=\"secondary\">TEST</button></div><div id=\"aiConnectionData\" class=\"connection-data\"></div><div class=\"setup-steps\"><b>PC setup</b><span>1. Install Stockfish on the PC.</span><span>2. Set <code>STOCKFISH_PATH</code> in the server .env.</span><span>3. Run <code>npm install</code>, then <code>npm start</code>.</span><span>4. Enter the PC LAN URL above.</span></div></div>\n"
"\n"
"        <div class=\"card connection-card chesscom-card\"><div class=\"connection-icon chesscom\">♟</div><div class=\"eyebrow\">OFFICIAL CONNECTOR</div><h3>Chess.com</h3><p>The public Chess.com API is read-only. It can provide public player/game data and PGN archives. Sending moves to Chess.com requires the official OAuth / Connected Board access path.</p><div class=\"connect-state-row\"><button id=\"chessStatus\" class=\"secondary\">CHECK STATUS</button><a class=\"button-link\" href=\"https://www.chess.com/clubs/about/chess-com-developer-community\" target=\"_blank\" rel=\"noreferrer\">OFFICIAL DEVELOPER COMMUNITY</a></div><label class=\"field\"><span>Public username</span><input id=\"chessUser\" placeholder=\"username\"></label><div class=\"row\"><button id=\"chessLookup\">LOAD PLAYER DATA</button><button id=\"chessImport\">IMPORT MONTHLY PGN</button></div><div class=\"field-inline\"><label>Year<input id=\"pgnYear\" type=\"number\" value=\"2026\"></label><label>Month<input id=\"pgnMonth\" type=\"number\" min=\"1\" max=\"12\" value=\"1\"></label></div><div id=\"chessResult\" class=\"connection-data\"></div><div class=\"official-note\"><b>Live game control:</b> Smart Chess will not ask for your Chess.com password or attempt to bypass Chess.com permissions. Use the official approved OAuth / Connected Board process for interactive access.</div></div>\n"
"\n"
"        <div class=\"card connection-card\"><div class=\"connection-icon net\">⌁</div><div class=\"eyebrow\">ONLINE / BOARD ↔ BOARD</div><h3>Validated rooms</h3><p>The included WebSocket server validates received moves using the same chess rules engine before broadcasting state.</p><label class=\"field\"><span>WebSocket URL</span><input id=\"wsUrl\" placeholder=\"ws://192.168.1.50:8080/ws\"></label><label class=\"field\"><span>Room code</span><input id=\"roomCode\" placeholder=\"SMARTV10\"></label><label class=\"field\"><span>Player name</span><input id=\"onlineName\" placeholder=\"Player\"></label><button id=\"connectOnline\" class=\"primary full\">CONNECT ONLINE ROOM</button><div id=\"onlineState\" class=\"connection-data\"></div></div>\n"
"\n"
"        <div class=\"card connection-card\"><div class=\"connection-icon board\">8×8</div><div class=\"eyebrow\">PHYSICAL BOARD</div><h3>ESP32 connection</h3><p>For direct board control, connect your PC or phone to the Smart Chess access point.</p><label class=\"field\"><span>Board URL</span><input id=\"boardUrl\" placeholder=\"http://192.168.4.1\"></label><div class=\"row\"><button id=\"testBoard\" class=\"secondary\">TEST BOARD</button><button id=\"saveBoard\" class=\"primary\">SAVE BOARD</button></div><div class=\"connection-data\"><b>SSID:</b> SmartChess-Test<br><b>Password:</b> 12345678<br><b>Default IP:</b> 192.168.4.1<br><b>App:</b> /app</div><div class=\"setup-steps\"><b>Phone + Hub note</b><span>The ESP32 AP directly controls the board. For phone access to a PC-hosted Hub/AI service, the phone and PC must be on a network that can reach the Hub.</span></div></div>\n"
"      </div>\n"
"    </section>\n"
"\n"
"    <section id=\"page-profile\" class=\"page\"><div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">PROFILE / PROGRESS</div><h2>Know what you are improving.</h2><p>Local player data and training statistics remain in your browser until you connect an external service.</p></div></div><div class=\"profile-hero card\"><div><div class=\"eyebrow\">PLAYER</div><h3 id=\"profilePlayerName\">Cyan</h3><p class=\"muted\">Smart Chess local profile</p></div><div id=\"profileStats\" class=\"profile-stat-grid\"></div></div></section>\n"
"\n"
"    <section id=\"page-settings\" class=\"page\"><div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">SETTINGS</div><h2>Configure the Smart Chess experience.</h2><p>Chess assistance, players, language, sound and hardware behavior.</p></div></div><div class=\"settings-grid\">\n"
"      <div class=\"card\"><div class=\"eyebrow\">CHESS EXPERIENCE</div><h3>Assistance</h3><label class=\"field\"><span>Assistance level</span><select id=\"assistance\"><option value=\"beginner\">Level 1 • Beginner</option><option value=\"hints\">Level 2 • Hints</option><option value=\"advanced\">Level 3 • Advanced</option><option value=\"pure\">Level 4 • Pure Chess</option></select></label><label class=\"field\"><span>Language</span><select id=\"language\"><option value=\"English\">English</option><option value=\"Arabic\">العربية</option></select></label><label class=\"field\"><span>Theme</span><select id=\"theme\"><option value=\"prime\">Prime Dark</option><option value=\"midnight\">Midnight</option><option value=\"frost\">Frost</option></select></label><label class=\"field\"><span>Side mapping</span><select id=\"cyanWhite\"><option value=\"true\">Cyan = White</option><option value=\"false\">Cyan = Black</option></select></label><label class=\"check\"><input id=\"sound\" type=\"checkbox\" checked><span>Enable sound effects</span></label></div>\n"
"      <div class=\"card\"><div class=\"eyebrow\">PLAYERS</div><h3>Names + AI</h3><label class=\"field\"><span>Cyan / White</span><input id=\"whiteName\" value=\"Cyan\"></label><label class=\"field\"><span>Gray / Black</span><input id=\"blackName\" value=\"Gray\"></label><label class=\"field\"><span>AI difficulty</span><select id=\"aiDifficulty\"><option>Beginner</option><option>Easy</option><option>Medium</option><option>Hard</option><option>Expert</option></select></label></div>\n"
"      <div class=\"card\"><div class=\"eyebrow\">HARDWARE</div><h3>LED behavior</h3><label class=\"field\"><span>Brightness <b id=\"brightnessVal\">80</b></span><input id=\"brightness\" type=\"range\" min=\"1\" max=\"255\" value=\"80\"></label><button id=\"saveSettings\" class=\"primary full\">SAVE SETTINGS</button><button id=\"settingsClearLeds\" class=\"secondary full\">CLEAR ALL LEDS</button><div id=\"saveSettingsMessage\" class=\"message info\">Local settings and hardware brightness are independent.</div></div>\n"
"      <div class=\"card\"><div class=\"eyebrow\">NON-VOLATILE STORAGE</div><h3>Hard save</h3><p class=\"muted\">Animation palettes, custom animation data, brightness, speed, tracking, game colors and mappings are stored in ESP32 flash.</p><button id=\"hardSave\" class=\"primary full\">HARD SAVE HARDWARE STATE</button><div id=\"saveMessage\" class=\"message info\">Ready.</div></div>\n"
"    </div></section>\n"
"\n"
"    <section id=\"page-telemetry\" class=\"page\"><div class=\"page-headline\"><div><div class=\"eyebrow accent-text\">LIVE DIAGNOSTICS</div><h2>Technical board health and live data.</h2><p>This is the engineering view: sensors, LEDs, calibration, mapping, games, runtime and the raw ESP32 data stream.</p></div><span class=\"chip accent\">LIVE</span></div>\n"
"      <div class=\"telemetry-top-grid\">\n"
"        <div class=\"card telemetry-card\"><div class=\"section-head\"><div><div class=\"eyebrow\">BOARD</div><h3>Hardware</h3></div></div><pre id=\"telemetryPhysical\" class=\"telemetry-pre\">Waiting...</pre><div id=\"telemetrySensorMap\" class=\"telemetry-map\" aria-label=\"Sensor activity map\"></div></div>\n"
"        <div class=\"card telemetry-card\"><div class=\"section-head\"><div><div class=\"eyebrow\">LED ENGINE</div><h3>Lights + animation</h3></div></div><pre id=\"telemetryLED\" class=\"telemetry-pre\">Waiting...</pre><div id=\"telemetryLEDMap\" class=\"telemetry-map led-map-mini\" aria-label=\"LED color map\"></div></div>\n"
"        <div class=\"card telemetry-card\"><div class=\"section-head\"><div><div class=\"eyebrow\">MAPPING</div><h3>Calibration</h3></div></div><pre id=\"telemetryMap\" class=\"telemetry-pre\">Waiting...</pre></div>\n"
"        <div class=\"card telemetry-card\"><div class=\"section-head\"><div><div class=\"eyebrow\">GAME</div><h3>Runtime state</h3></div></div><pre id=\"telemetryGame\" class=\"telemetry-pre\">Waiting...</pre></div>\n"
"      </div>\n"
"      <div class=\"card full-span telemetry-raw\"><div class=\"section-head\"><div><div class=\"eyebrow\">ADVANCED DATA</div><h3>Raw /api/board response</h3></div><button class=\"secondary\" id=\"copyTelemetry\">COPY JSON</button></div><details class=\"diagnostic-details\"><summary>Show raw board JSON</summary><pre id=\"rawTelemetry\" class=\"telemetry-pre raw\">Waiting...</pre></details></div>\n"
"    </section>\n"
"\n"
"    <div id=\"legacyCompat\" hidden aria-hidden=\"true\">\n"
"      <button id=\"checkBoard\"></button><button id=\"sendHint\"></button><button id=\"clearLEDs\"></button><div id=\"connectStatus\"></div>\n"
"      <div id=\"turn\"></div><div id=\"gameState\"></div><div id=\"moveCount\"></div><div id=\"halfmove\"></div><div id=\"statusMsg\"></div><div id=\"boardConnection\"></div><div id=\"assistantLevel\"></div><div id=\"physicalState\"></div><div id=\"moveHistory\"></div><div id=\"replayControls\"></div><div id=\"fenBox\"></div><div id=\"brightnessValLegacy\"></div>\n"
"      <select id=\"checkBoard2\"></select><input id=\"brightnessLegacy\"><input id=\"saveSettingsLegacy\"><input id=\"clearLEDsLegacy\"><input id=\"sendHintLegacy\"><button id=\"startGame2\"></button><button id=\"resetGame2\"></button><div id=\"currentStatus\"></div>\n"
"    </div>\n"
"  </main>\n"
"</div>\n"
"<div id=\"toast\" class=\"toast\"></div>\n"
"<script>\n"
"/* Smart Chess - shared chess rules engine\n"
" * No external dependencies. Works in browser and Node.js.\n"
" * Board index = rank * 8 + file, rank 0 = rank 1, file 0 = A.\n"
" */\n"
"(function(root, factory){\n"
"  if(typeof module === 'object' && module.exports) module.exports = factory();\n"
"  else root.SmartChessEngine = factory();\n"
"})(typeof self !== 'undefined' ? self : this, function(){\n"
"  'use strict';\n"
"\n"
"  const FILES = 'abcdefgh';\n"
"  const START_FEN = 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1';\n"
"  const WHITE='w', BLACK='b';\n"
"  const CASTLE_WK=1, CASTLE_WQ=2, CASTLE_BK=4, CASTLE_BQ=8;\n"
"\n"
"  function scSquareIndex(file,rank){ return rank*8+file; }\n"
"  function fileOf(i){ return i & 7; }\n"
"  function rankOf(i){ return i >> 3; }\n"
"  function coord(i){ return FILES[fileOf(i)] + (rankOf(i)+1); }\n"
"  function parseCoord(s){\n"
"    if(!/^[a-h][1-8]$/.test(s)) return -1;\n"
"    return scSquareIndex(FILES.indexOf(s[0]), Number(s[1])-1);\n"
"  }\n"
"  function colorOf(p){ if(!p) return null; return p===p.toUpperCase()?WHITE:BLACK; }\n"
"  function typeOf(p){ return p ? p.toUpperCase() : null; }\n"
"  function opposite(c){ return c===WHITE?BLACK:WHITE; }\n"
"\n"
"  function cloneState(s){\n"
"    return {\n"
"      board: s.board.slice(), side:s.side, castling:s.castling, ep:s.ep,\n"
"      halfmove:s.halfmove, fullmove:s.fullmove,\n"
"      history:s.history.slice(), moveHistory:s.moveHistory.slice(), result:s.result||null,\n"
"      lastMove:s.lastMove?Object.assign({},s.lastMove):null\n"
"    };\n"
"  }\n"
"\n"
"  class Chess {\n"
"    constructor(fen){ this.reset(fen || START_FEN); }\n"
"    reset(fen){\n"
"      const p = fen.split(/\\s+/);\n"
"      if(p.length < 4) throw new Error('Invalid FEN');\n"
"      this.board = new Array(64).fill(null);\n"
"      const rows=p[0].split('/');\n"
"      if(rows.length!==8) throw new Error('Invalid FEN board');\n"
"      for(let fenRank=0;fenRank<8;fenRank++){\n"
"        let file=0;\n"
"        for(const ch of rows[fenRank]){\n"
"          if(/[1-8]/.test(ch)) file += Number(ch);\n"
"          else { if(file>=8) throw new Error('Invalid FEN row'); this.board[scSquareIndex(file,7-fenRank)]=ch; file++; }\n"
"        }\n"
"        if(file!==8) throw new Error('Invalid FEN row width');\n"
"      }\n"
"      this.side=p[1]==='b'?BLACK:WHITE;\n"
"      this.castling=0;\n"
"      if(p[2].includes('K')) this.castling|=CASTLE_WK;\n"
"      if(p[2].includes('Q')) this.castling|=CASTLE_WQ;\n"
"      if(p[2].includes('k')) this.castling|=CASTLE_BK;\n"
"      if(p[2].includes('q')) this.castling|=CASTLE_BQ;\n"
"      this.ep=p[3]==='-'?-1:parseCoord(p[3]);\n"
"      this.halfmove=Number(p[4]||0);\n"
"      this.fullmove=Number(p[5]||1);\n"
"      this.history=[this.positionKey()];\n"
"      this.moveHistory=[];\n"
"      this.result=null;\n"
"      this.lastMove=null;\n"
"      this.updateResult();\n"
"    }\n"
"\n"
"    fen(){\n"
"      const rows=[];\n"
"      for(let r=7;r>=0;r--){\n"
"        let row='',empty=0;\n"
"        for(let f=0;f<8;f++){\n"
"          const p=this.board[scSquareIndex(f,r)];\n"
"          if(!p) empty++;\n"
"          else { if(empty){row+=empty;empty=0;} row+=p; }\n"
"        }\n"
"        if(empty) row+=empty;\n"
"        rows.push(row);\n"
"      }\n"
"      let c='';\n"
"      if(this.castling&CASTLE_WK)c+='K'; if(this.castling&CASTLE_WQ)c+='Q';\n"
"      if(this.castling&CASTLE_BK)c+='k'; if(this.castling&CASTLE_BQ)c+='q';\n"
"      if(!c)c='-';\n"
"      return rows.join('/')+' '+this.side+' '+c+' '+(this.ep>=0?coord(this.ep):'-')+' '+this.halfmove+' '+this.fullmove;\n"
"    }\n"
"\n"
"    positionKey(){\n"
"      let b='';\n"
"      for(let i=0;i<64;i++) b+=this.board[i]||'.';\n"
"      let c='';\n"
"      if(this.castling&CASTLE_WK)c+='K'; if(this.castling&CASTLE_WQ)c+='Q';\n"
"      if(this.castling&CASTLE_BK)c+='k'; if(this.castling&CASTLE_BQ)c+='q';\n"
"      // En-passant square only matters for repetition if a legal EP capture exists.\n"
"      let epPart='-';\n"
"      if(this.ep>=0 && this.hasLegalEnPassantCapture()) epPart=coord(this.ep);\n"
"      return b+' '+this.side+' '+(c||'-')+' '+epPart;\n"
"    }\n"
"\n"
"    hasLegalEnPassantCapture(){\n"
"      if(this.ep<0)return false;\n"
"      const r=rankOf(this.ep), f=fileOf(this.ep);\n"
"      const pawnRank=this.side===WHITE?r-1:r+1;\n"
"      if(pawnRank<0||pawnRank>7)return false;\n"
"      for(const df of [-1,1]){\n"
"        const ff=f+df;\n"
"        if(ff<0||ff>7)continue;\n"
"        const from=scSquareIndex(ff,pawnRank);\n"
"        if(this.board[from]===(this.side===WHITE?'P':'p')){\n"
"          const m={from,to:this.ep,enPassant:true,capture:true,promotion:null};\n"
"          if(this.isLegalAfter(m)) return true;\n"
"        }\n"
"      }\n"
"      return false;\n"
"    }\n"
"\n"
"    findKing(side){ return this.board.findIndex(p=>p===(side===WHITE?'K':'k')); }\n"
"\n"
"    isSquareAttacked(target, bySide){\n"
"      const tr=rankOf(target), tf=fileOf(target);\n"
"      // pawn attacks\n"
"      const pawn=bySide===WHITE?'P':'p';\n"
"      const pr=bySide===WHITE?tr-1:tr+1;\n"
"      if(pr>=0&&pr<8){\n"
"        for(const df of [-1,1]){ const f=tf+df; if(f>=0&&f<8&&this.board[scSquareIndex(f,pr)]===pawn) return true; }\n"
"      }\n"
"      // knights\n"
"      const knight=bySide===WHITE?'N':'n';\n"
"      for(const [df,dr] of [[1,2],[2,1],[-1,2],[-2,1],[1,-2],[2,-1],[-1,-2],[-2,-1]]){\n"
"        const f=tf+df,r=tr+dr; if(f>=0&&f<8&&r>=0&&r<8&&this.board[scSquareIndex(f,r)]===knight)return true;\n"
"      }\n"
"      // king\n"
"      const king=bySide===WHITE?'K':'k';\n"
"      for(const df of [-1,0,1])for(const dr of [-1,0,1]){\n"
"        if(!df&&!dr)continue; const f=tf+df,r=tr+dr;\n"
"        if(f>=0&&f<8&&r>=0&&r<8&&this.board[scSquareIndex(f,r)]===king)return true;\n"
"      }\n"
"      // bishops/queens\n"
"      const bishop=bySide===WHITE?'B':'b', queen=bySide===WHITE?'Q':'q';\n"
"      for(const [df,dr] of [[1,1],[1,-1],[-1,1],[-1,-1]]){\n"
"        let f=tf+df,r=tr+dr;\n"
"        while(f>=0&&f<8&&r>=0&&r<8){\n"
"          const p=this.board[scSquareIndex(f,r)]; if(p){ if(p===bishop||p===queen)return true; break; } f+=df;r+=dr;\n"
"        }\n"
"      }\n"
"      // rooks/queens\n"
"      const rook=bySide===WHITE?'R':'r';\n"
"      for(const [df,dr] of [[1,0],[-1,0],[0,1],[0,-1]]){\n"
"        let f=tf+df,r=tr+dr;\n"
"        while(f>=0&&f<8&&r>=0&&r<8){\n"
"          const p=this.board[scSquareIndex(f,r)]; if(p){ if(p===rook||p===queen)return true; break; } f+=df;r+=dr;\n"
"        }\n"
"      }\n"
"      return false;\n"
"    }\n"
"\n"
"    inCheck(side=this.side){\n"
"      const k=this.findKing(side); return k>=0 ? this.isSquareAttacked(k,opposite(side)) : true;\n"
"    }\n"
"\n"
"    pseudoMoves(side=this.side){\n"
"      const moves=[];\n"
"      for(let from=0;from<64;from++){\n"
"        const p=this.board[from]; if(!p||colorOf(p)!==side)continue;\n"
"        const t=typeOf(p), f=fileOf(from), r=rankOf(from);\n"
"        if(t==='P'){\n"
"          const dir=side===WHITE?1:-1, startRank=side===WHITE?1:6, promoRank=side===WHITE?7:0;\n"
"          const oneR=r+dir;\n"
"          if(oneR>=0&&oneR<8){\n"
"            const one=scSquareIndex(f,oneR);\n"
"            if(!this.board[one]){\n"
"              if(oneR===promoRank) for(const pr of ['Q','R','B','N']) moves.push({from,to:one,promotion:side===WHITE?pr:pr.toLowerCase(),capture:false,enPassant:false});\n"
"              else {\n"
"                moves.push({from,to:one,promotion:null,capture:false,enPassant:false});\n"
"                if(r===startRank){ const two=scSquareIndex(f,r+2*dir); if(!this.board[two]) moves.push({from,to:two,promotion:null,capture:false,enPassant:false}); }\n"
"              }\n"
"            }\n"
"            for(const df of [-1,1]){\n"
"              const ff=f+df; if(ff<0||ff>7)continue; const to=scSquareIndex(ff,oneR), dest=this.board[to];\n"
"              if(dest&&colorOf(dest)!==side){\n"
"                if(oneR===promoRank) for(const pr of ['Q','R','B','N']) moves.push({from,to,promotion:side===WHITE?pr:pr.toLowerCase(),capture:true,enPassant:false});\n"
"                else moves.push({from,to,promotion:null,capture:true,enPassant:false});\n"
"              } else if(to===this.ep){ moves.push({from,to,promotion:null,capture:true,enPassant:true}); }\n"
"            }\n"
"          }\n"
"        } else if(t==='N'){\n"
"          for(const [df,dr] of [[1,2],[2,1],[-1,2],[-2,1],[1,-2],[2,-1],[-1,-2],[-2,-1]]){\n"
"            const ff=f+df,rr=r+dr; if(ff<0||ff>7||rr<0||rr>7)continue; const to=scSquareIndex(ff,rr),d=this.board[to]; if(!d||colorOf(d)!==side)moves.push({from,to,promotion:null,capture:!!d,enPassant:false});\n"
"          }\n"
"        } else if(t==='B'||t==='R'||t==='Q'){\n"
"          const dirs=[]; if(t==='B'||t==='Q') dirs.push([1,1],[1,-1],[-1,1],[-1,-1]); if(t==='R'||t==='Q') dirs.push([1,0],[-1,0],[0,1],[0,-1]);\n"
"          for(const [df,dr] of dirs){ let ff=f+df,rr=r+dr; while(ff>=0&&ff<8&&rr>=0&&rr<8){ const to=scSquareIndex(ff,rr),d=this.board[to]; if(!d)moves.push({from,to,promotion:null,capture:false,enPassant:false}); else {if(colorOf(d)!==side)moves.push({from,to,promotion:null,capture:true,enPassant:false}); break;} ff+=df;rr+=dr; } }\n"
"        } else if(t==='K'){\n"
"          for(const df of [-1,0,1])for(const dr of [-1,0,1]){if(!df&&!dr)continue; const ff=f+df,rr=r+dr;if(ff<0||ff>7||rr<0||rr>7)continue;const to=scSquareIndex(ff,rr),d=this.board[to];if(!d||colorOf(d)!==side)moves.push({from,to,promotion:null,capture:!!d,enPassant:false});}\n"
"          if(side===WHITE&&from===scSquareIndex(4,0)&&!this.inCheck(WHITE)){\n"
"            if((this.castling&CASTLE_WK)&&this.board[scSquareIndex(5,0)]===null&&this.board[scSquareIndex(6,0)]===null&&this.board[scSquareIndex(7,0)]==='R'&& !this.isSquareAttacked(scSquareIndex(5,0),BLACK)&&!this.isSquareAttacked(scSquareIndex(6,0),BLACK)) moves.push({from,to:scSquareIndex(6,0),promotion:null,capture:false,enPassant:false,castle:'K'});\n"
"            if((this.castling&CASTLE_WQ)&&this.board[scSquareIndex(1,0)]===null&&this.board[scSquareIndex(2,0)]===null&&this.board[scSquareIndex(3,0)]===null&&this.board[scSquareIndex(0,0)]==='R'&& !this.isSquareAttacked(scSquareIndex(3,0),BLACK)&&!this.isSquareAttacked(scSquareIndex(2,0),BLACK)) moves.push({from,to:scSquareIndex(2,0),promotion:null,capture:false,enPassant:false,castle:'Q'});\n"
"          }\n"
"          if(side===BLACK&&from===scSquareIndex(4,7)&&!this.inCheck(BLACK)){\n"
"            if((this.castling&CASTLE_BK)&&this.board[scSquareIndex(5,7)]===null&&this.board[scSquareIndex(6,7)]===null&&this.board[scSquareIndex(7,7)]==='r'&& !this.isSquareAttacked(scSquareIndex(5,7),WHITE)&&!this.isSquareAttacked(scSquareIndex(6,7),WHITE)) moves.push({from,to:scSquareIndex(6,7),promotion:null,capture:false,enPassant:false,castle:'K'});\n"
"            if((this.castling&CASTLE_BQ)&&this.board[scSquareIndex(1,7)]===null&&this.board[scSquareIndex(2,7)]===null&&this.board[scSquareIndex(3,7)]===null&&this.board[scSquareIndex(0,7)]==='r'&& !this.isSquareAttacked(scSquareIndex(3,7),WHITE)&&!this.isSquareAttacked(scSquareIndex(2,7),WHITE)) moves.push({from,to:scSquareIndex(2,7),promotion:null,capture:false,enPassant:false,castle:'Q'});\n"
"          }\n"
"        }\n"
"      }\n"
"      return moves;\n"
"    }\n"
"\n"
"    isLegalAfter(move){\n"
"      const u=this.makeMove(move,false);\n"
"      const ok=!this.inCheck(opposite(this.side));\n"
"      this.unmakeMove(u);\n"
"      return ok;\n"
"    }\n"
"\n"
"    legalMoves(side=this.side){ const enemyKing=side===WHITE?'k':'K'; return this.pseudoMoves(side).filter(m=>this.board[m.to]!==enemyKing && this.isLegalAfter(m)); }\n"
"\n"
"    makeMove(move, record=true){\n"
"      const u={move,fromPiece:this.board[move.from],toPiece:this.board[move.to],castling:this.castling,ep:this.ep,halfmove:this.halfmove,fullmove:this.fullmove,side:this.side,lastMove:this.lastMove};\n"
"      const p=this.board[move.from];\n"
"      this.board[move.from]=null;\n"
"      this.board[move.to]=move.promotion||p;\n"
"      if(move.enPassant){\n"
"        const cap=move.to + (this.side===WHITE?-8:8); u.epCapture={idx:cap,piece:this.board[cap]}; this.board[cap]=null;\n"
"      }\n"
"      if(move.castle==='K'){\n"
"        const rf=this.side===WHITE?scSquareIndex(7,0):scSquareIndex(7,7), rt=this.side===WHITE?scSquareIndex(5,0):scSquareIndex(5,7); u.rook={from:rf,to:rt,piece:this.board[rf]}; this.board[rt]=this.board[rf]; this.board[rf]=null;\n"
"      } else if(move.castle==='Q'){\n"
"        const rf=this.side===WHITE?scSquareIndex(0,0):scSquareIndex(0,7), rt=this.side===WHITE?scSquareIndex(3,0):scSquareIndex(3,7); u.rook={from:rf,to:rt,piece:this.board[rf]}; this.board[rt]=this.board[rf]; this.board[rf]=null;\n"
"      }\n"
"      this.updateCastleRightsOnMove(move,p,u.toPiece);\n"
"      this.ep=-1;\n"
"      if(typeOf(p)==='P' && Math.abs(rankOf(move.to)-rankOf(move.from))===2) this.ep=scSquareIndex(fileOf(move.from),(rankOf(move.from)+rankOf(move.to))/2);\n"
"      this.halfmove=(typeOf(p)==='P'||u.toPiece||move.enPassant)?0:this.halfmove+1;\n"
"      if(this.side===BLACK)this.fullmove++;\n"
"      this.side=opposite(this.side);\n"
"      if(record){\n"
"        const san=this.sanFromMove(move);\n"
"        this.moveHistory.push({from:move.from,to:move.to,promotion:move.promotion||null,san,fen:this.fen(),capture:!!(u.toPiece||move.enPassant),check:false,checkmate:false});\n"
"        this.lastMove=Object.assign({},move,{san});\n"
"        this.history.push(this.positionKey());\n"
"        this.updateResult();\n"
"        const last=this.moveHistory[this.moveHistory.length-1];\n"
"        last.check=this.inCheck(this.side);\n"
"        last.checkmate=this.result==='checkmate';\n"
"      }\n"
"      return u;\n"
"    }\n"
"\n"
"    unmakeMove(u){\n"
"      const m=u.move;\n"
"      this.side=opposite(this.side);\n"
"      if(this.side===BLACK)this.fullmove--;\n"
"      this.castling=u.castling; this.ep=u.ep; this.halfmove=u.halfmove; this.fullmove=u.fullmove; this.lastMove=u.lastMove;\n"
"      this.board[m.from]=u.fromPiece; this.board[m.to]=u.toPiece;\n"
"      if(u.epCapture)this.board[u.epCapture.idx]=u.epCapture.piece;\n"
"      if(u.rook){ this.board[u.rook.from]=u.rook.piece; this.board[u.rook.to]=null; }\n"
"    }\n"
"\n"
"    updateCastleRightsOnMove(move,p,captured){\n"
"      if(p==='K')this.castling &= ~(CASTLE_WK|CASTLE_WQ);\n"
"      if(p==='k')this.castling &= ~(CASTLE_BK|CASTLE_BQ);\n"
"      if(move.from===scSquareIndex(0,0)||move.to===scSquareIndex(0,0))this.castling&=~CASTLE_WQ;\n"
"      if(move.from===scSquareIndex(7,0)||move.to===scSquareIndex(7,0))this.castling&=~CASTLE_WK;\n"
"      if(move.from===scSquareIndex(0,7)||move.to===scSquareIndex(0,7))this.castling&=~CASTLE_BQ;\n"
"      if(move.from===scSquareIndex(7,7)||move.to===scSquareIndex(7,7))this.castling&=~CASTLE_BK;\n"
"      void captured;\n"
"    }\n"
"\n"
"    sanFromMove(move){\n"
"      if(move.castle==='K') return this.sanSuffix('O-O');\n"
"      if(move.castle==='Q') return this.sanSuffix('O-O-O');\n"
"      const p=this.board[move.to];\n"
"      const piece=typeOf(p);\n"
"      const capture=!!(move.capture||move.enPassant);\n"
"      let s='';\n"
"      if(piece!=='P'){\n"
"        s+=piece;\n"
"        const competitors=this.legalMoves(opposite(this.side)).filter(m=>m.to===move.to && m.from!==move.from && typeOf(this.board[m.from])===piece);\n"
"        if(competitors.length){\n"
"          const sameFile=competitors.some(m=>fileOf(m.from)===fileOf(move.from));\n"
"          const sameRank=competitors.some(m=>rankOf(m.from)===rankOf(move.from));\n"
"          if(!sameFile)s+=FILES[fileOf(move.from)];\n"
"          else if(!sameRank)s+=String(rankOf(move.from)+1);\n"
"          else s+=coord(move.from);\n"
"        }\n"
"      } else if(capture){ s+=FILES[fileOf(move.from)]; }\n"
"      if(capture)s+='x';\n"
"      s+=coord(move.to);\n"
"      if(move.promotion)s+='='+typeOf(move.promotion);\n"
"      return this.sanSuffix(s);\n"
"    }\n"
"\n"
"    sanSuffix(base){\n"
"      const side=this.side; // already switched after move, so this.side is opponent\n"
"      if(this.inCheck(side)){\n"
"        if(this.legalMoves(side).length===0)return base+'#';\n"
"        return base+'+';\n"
"      }\n"
"      return base;\n"
"    }\n"
"\n"
"    playUci(uci){\n"
"      if(!/^[a-h][1-8][a-h][1-8][qrbnQRBN]?$/.test(uci))throw new Error('Invalid move string');\n"
"      const from=parseCoord(uci.slice(0,2)), to=parseCoord(uci.slice(2,4));\n"
"      const promotion=uci.length>4? (this.side===WHITE?uci[4].toUpperCase():uci[4].toLowerCase()) : null;\n"
"      const move=this.legalMoves().find(m=>m.from===from&&m.to===to&&(m.promotion||null)===promotion);\n"
"      if(!move)throw new Error('Illegal move');\n"
"      this.makeMove(move,true); return move;\n"
"    }\n"
"\n"
"    updateResult(){\n"
"      const moves=this.legalMoves(this.side);\n"
"      if(moves.length===0){ this.result=this.inCheck(this.side)?'checkmate':'stalemate'; return; }\n"
"      if(this.halfmove>=100){this.result='50-move';return;}\n"
"      const key=this.positionKey(); let n=0; for(const k of this.history)if(k===key)n++; if(n>=3){this.result='threefold';return;}\n"
"      if(this.insufficientMaterial()){this.result='insufficient';return;}\n"
"      this.result=this.inCheck(this.side)?'check':null;\n"
"    }\n"
"\n"
"    insufficientMaterial(){\n"
"      const pieces=[];\n"
"      for(let i=0;i<64;i++){const p=this.board[i];if(p&&typeOf(p)!=='K')pieces.push({p,i});}\n"
"      if(pieces.length===0)return true;\n"
"      if(pieces.some(x=>['P','Q','R'].includes(typeOf(x.p))))return false;\n"
"      if(pieces.length===1&&['B','N'].includes(typeOf(pieces[0].p)))return true;\n"
"      if(pieces.every(x=>typeOf(x.p)==='B')){\n"
"        const colors=pieces.map(x=>(fileOf(x.i)+rankOf(x.i))&1); return colors.every(c=>c===colors[0]);\n"
"      }\n"
"      if(pieces.length===2&&pieces.every(x=>typeOf(x.p)==='N'))return true;\n"
"      return false;\n"
"    }\n"
"\n"
"    pieceAtSquare(c){ const i=parseCoord(c); return i>=0?this.board[i]:null; }\n"
"    legalMovesFrom(c){ const i=typeof c==='number'?c:parseCoord(c); return this.legalMoves().filter(m=>m.from===i); }\n"
"    toPosition(){ return this.board.slice(); }\n"
"    snapshot(){ return {fen:this.fen(),board:this.board.slice(),side:this.side,castling:this.castling,ep:this.ep,halfmove:this.halfmove,fullmove:this.fullmove,result:this.result,lastMove:this.lastMove,moveHistory:this.moveHistory.slice()}; }\n"
"  }\n"
"\n"
"  return {Chess, START_FEN, coord, parseCoord, fileOf, rankOf, WHITE, BLACK};\n"
"});\n"
"\n"
"</script>\n"
"<script>\n"
"/* Smart Chess application layer.\n"
" * Talks to an ESP32 board over HTTP and optionally to a WebSocket multiplayer server.\n"
" */\n"
"(() => {\n"
"  'use strict';\n"
"  const {Chess, coord, parseCoord, fileOf, rankOf, WHITE, BLACK, START_FEN} = window.SmartChessEngine;\n"
"  const $ = id => document.getElementById(id);\n"
"\n"
"  const PIECE_GLYPH=Object.freeze({K:String.fromCodePoint(9812),Q:String.fromCodePoint(9813),R:String.fromCodePoint(9814),B:String.fromCodePoint(9815),N:String.fromCodePoint(9816),P:String.fromCodePoint(9817),k:String.fromCodePoint(9818),q:String.fromCodePoint(9819),r:String.fromCodePoint(9820),b:String.fromCodePoint(9821),n:String.fromCodePoint(9822),p:String.fromCodePoint(9823)});\n"
"  const INITIAL = {\n"
"    boardUrl: localStorage.getItem('sc.boardUrl') || localStorage.getItem('smartchess.boardUrl') || location.origin,\n"
"    assistance: localStorage.getItem('smartchess.assistance') || 'beginner',\n"
"    brightness: Number(localStorage.getItem('smartchess.brightness') || 80),\n"
"    sound: localStorage.getItem('smartchess.sound') !== '0',\n"
"    language: localStorage.getItem('smartchess.language') || 'English',\n"
"    cyanIsWhite: localStorage.getItem('smartchess.cyanIsWhite') !== '0',\n"
"    aiDifficulty: localStorage.getItem('smartchess.aiDifficulty') || 'Beginner',\n"
"    whiteName: localStorage.getItem('smartchess.whiteName') || 'Cyan',\n"
"    blackName: localStorage.getItem('smartchess.blackName') || 'Gray'\n"
"  };\n"
"\n"
"  const S = {\n"
"    page:'home', engine:new Chess(), selected:-1, legal:[], physical:null, expected:null,\n"
"    connected:false, lastBoardFingerprint:'', pendingFrom:-1, pendingStarted:0, currentLesson:null, currentPuzzle:null, replay:null,\n"
"    statusMessage:'Ready', mismatch:false, gameStarted:false, screenTest:false,\n"
"    promotionPending:null, online:{ws:null,room:'',role:'',connected:false},\n"
"    timer:{enabled:false,w:600,b:600,last:0,running:false}, stats:JSON.parse(localStorage.getItem('smartchess.stats')||'{\"games\":0,\"wins\":0,\"losses\":0,\"draws\":0,\"puzzles\":0,\"puzzleSuccess\":0,\"training\":0}'),\n"
"    savedGames:JSON.parse(localStorage.getItem('smartchess.games')||'[]'), settings:INITIAL,\n"
"    chessCom:{player:null,pgn:''}\n"
"  };\n"
"\n"
"  const navPages = ['home','play','led','learn','puzzles','games','analysis','connect','profile','settings','telemetry'];\n"
"  const pageNames = {home:'Command Center',play:'Play Chess',led:'Light Lab',learn:'Learn',puzzles:'Puzzles',games:'Game Archive',analysis:'Analysis',connect:'Connections',profile:'Profile',settings:'Settings',telemetry:'Live Diagnostics'};\n"
"\n"
"  function esc(s){return String(s).replace(/[&<>\"']/g,m=>({'&':'&amp;','<':'&lt;','>':'&gt;','\"':'&quot;',\"'\":'&#39;'}[m]));}\n"
"  function boardFingerprint(b){return b?b.map(r=>r.map(Boolean).map(x=>x?'1':'0').join('')).join('|'):'';}\n"
"  function occFromEngine(){return S.engine.board.map(Boolean);}\n"
"  function sqName(i){return i<0?'--':coord(i).toUpperCase();}\n"
"  function status(text,kind=''){S.statusMessage=text; const e=$('statusMsg'); if(e){e.textContent=text;e.className='small '+kind;} renderTop();}\n"
"  function renderTop(){ const lang=localStorage.getItem('smartchess.language')||'English'; $('pageTitle').textContent=lang==='Arabic'?(NAV_AR?.[S.page]||pageNames[S.page]||'Smart Chess'):(pageNames[S.page]||'Smart Chess'); const chip=$('connectStatus'); chip.textContent=S.connected?'BOARD CONNECTED':'BOARD DISCONNECTED'; chip.className='statusChip '+(S.connected?'ok':'bad'); }\n"
"\n"
"  function navigate(page){S.page=page;document.querySelectorAll('.page').forEach(p=>p.classList.toggle('active',p.id==='page-'+page));document.querySelectorAll('.nav-item').forEach(b=>b.classList.toggle('active',b.dataset.page===page)); if(page==='games')renderGames();if(page==='profile')renderProfile();if(page==='analysis')renderAnalysis();if(page==='settings')renderSettings();}\n"
"\n"
"  function buildBoard(containerId='chessBoard'){\n"
"    const el=$(containerId); if(!el)return; el.innerHTML='';\n"
"    // Hard-lock the board to exactly 8 equal columns x 8 equal rows.\n"
"    // Both width and height are controlled by CSS so every cell is a true square.\n"
"    el.style.display='grid';\n"
"    el.style.gridTemplateColumns='repeat(8,minmax(0,1fr))';\n"
"    el.style.gridTemplateRows='repeat(8,minmax(0,1fr))';\n"
"    el.style.gridAutoFlow='row';\n"
"    el.style.aspectRatio='1 / 1';\n"
"    el.style.width='min(100%, 720px)';\n"
"    el.style.height='auto';\n"
"\n"
"    const isPhysicalMirror = containerId === 'homeBoard';\n"
"    const physical = S.physical?.matrix || null;\n"
"    const ledColors = S.physical?.ledColors || null;\n"
"\n"
"    // Rank 8 at top, rank 1 at bottom.\n"
"    for(let r=7;r>=0;r--){\n"
"      for(let f=0;f<8;f++){\n"
"        const i=r*8+f;\n"
"        const b=document.createElement('button');\n"
"        b.type='button';\n"
"        b.className='sq '+(((r+f)&1)?'dark':'light');\n"
"        b.dataset.i=i;\n"
"        b.style.aspectRatio='1 / 1';\n"
"        const occupied = !!physical?.[r]?.[f];\n"
"        const expectedPiece = S.engine.board[i];\n"
"\n"
"        if(isPhysicalMirror){\n"
"          // Home is the sensor mirror: never invent a chess piece where the sensor is empty.\n"
"          if(occupied){\n"
"            if(expectedPiece){\n"
"              const sp=document.createElement('span');\n"
"              sp.className='piece '+(expectedPiece===expectedPiece.toUpperCase()?'cyanPiece':'grayPiece');\n"
"              sp.textContent=PIECE_GLYPH[expectedPiece]||expectedPiece;\n"
"              b.appendChild(sp);\n"
"            }else{\n"
"              const sp=document.createElement('span');\n"
"              sp.className='physicalMagnet';\n"
"              sp.textContent='◆';\n"
"              b.appendChild(sp);\n"
"            }\n"
"          }\n"
"          const glow=ledColors?.[i];\n"
"          if(glow && glow!=='#05090C' && glow!=='#000000'){\n"
"            b.style.boxShadow=`inset 0 0 0 2px ${glow}, inset 0 0 24px ${glow}55`;\n"
"          }\n"
"        }else{\n"
"          if(expectedPiece){const sp=document.createElement('span');sp.className='piece '+(expectedPiece===expectedPiece.toUpperCase()?'cyanPiece':'grayPiece');sp.textContent=PIECE_GLYPH[expectedPiece]||expectedPiece;b.appendChild(sp)}\n"
"          if(i===S.selected)b.classList.add('selected');\n"
"          if(i===S.pendingFrom && S.pendingFrom>=0)b.classList.add('lifted');\n"
"          const lm=S.engine.lastMove; if(lm&&(i===lm.from||i===lm.to))b.classList.add('last');\n"
"          if(S.legal.includes(i))b.classList.add(S.engine.board[i]?'capture':'legal');\n"
"          if(S.engine.inCheck(WHITE)&&S.engine.board[i]==='K')b.classList.add('check');\n"
"          if(S.engine.inCheck(BLACK)&&S.engine.board[i]==='k')b.classList.add('check');\n"
"          if(S.mismatch && i===S.pendingFrom)b.classList.add('mismatch');\n"
"        }\n"
"        const c=document.createElement('span');c.className='coord';c.textContent=FILES_LABEL(r,f);b.appendChild(c);\n"
"        b.addEventListener('click',()=>screenSquare(i));\n"
"        el.appendChild(b);\n"
"      }\n"
"    }\n"
"  }\n"
"  function FILES_LABEL(r,f){return (f===0?String(r+1):'')+(r===0?'abcdefgh'[f]:'');}\n"
"\n"
"  function renderStats(){\n"
"    $('turn').textContent=S.engine.side===WHITE?'Cyan':'Gray';\n"
"    $('gameState').textContent=S.engine.result||'playing';\n"
"    $('moveCount').textContent=S.engine.moveHistory.length;\n"
"    $('halfmove').textContent=S.engine.halfmove;\n"
"    $('fenBox').textContent=S.engine.fen();\n"
"    $('boardConnection').textContent=S.connected?'Live physical board':'Simulation only';\n"
"    $('assistantLevel').textContent=S.settings.assistance;\n"
"    renderClocks();\n"
"  }\n"
"\n"
"  function pieceFullName(p){const side=p===p.toUpperCase()?'White':'Black';const names={P:'Pawn',N:'Knight',B:'Bishop',R:'Rook',Q:'Queen',K:'King'};return side+' '+(names[p.toUpperCase()]||'Piece');}\n"
"  function pieceGlyph(p){return PIECE_GLYPH[p]||'';}\n"
"  function renderReadablePosition(){\n"
"    const box=$('playFen'); if(!box)return; box.innerHTML='';\n"
"    const mini=document.createElement('div'); mini.className='position-mini-board';\n"
"    for(let r=7;r>=0;r--){ for(let f=0;f<8;f++){\n"
"      const i=r*8+f,p=S.engine.board[i],cell=document.createElement('div');\n"
"      cell.className='position-mini-cell '+(((r+f)&1)?'dark':'light')+(p?' occupied':' empty');\n"
"      const lab=document.createElement('small');lab.textContent=String.fromCharCode(65+f)+(r+1);cell.appendChild(lab);\n"
"      const glyph=document.createElement('span');glyph.className='mini-piece '+(p?(p===p.toUpperCase()?'white-piece':'black-piece'):'');glyph.textContent=p?pieceGlyph(p):'';glyph.title=p?pieceFullName(p):'Empty';cell.appendChild(glyph);\n"
"      mini.appendChild(cell);\n"
"    }}\n"
"    box.appendChild(mini);\n"
"    const list=document.createElement('div');list.className='position-inventory';\n"
"    const occupied=[];for(let r=7;r>=0;r--)for(let f=0;f<8;f++){const i=r*8+f,p=S.engine.board[i];if(p)occupied.push(String.fromCharCode(65+f)+(r+1)+' - '+pieceFullName(p));}\n"
"    list.innerHTML='<div class=\"inventory-title\">PIECES ON BOARD</div><div class=\"inventory-items\">'+(occupied.length?occupied.map(x=>'<span>'+esc(x)+'</span>').join(''):'<span>No pieces</span>')+'</div>';\n"
"    box.appendChild(list);\n"
"    const raw=$('playFenRaw');if(raw)raw.textContent='Technical FEN (for export / engine use): '+S.engine.fen();\n"
"  }\n"
"  function renderHistory(){\n"
"    const targets=[$('moveHistory'),$('playHistory')].filter(Boolean);\n"
"    targets.forEach(h=>{h.innerHTML='';});\n"
"    const moves=S.engine.moveHistory;\n"
"    for(let i=0;i<moves.length;i+=2){const w=moves[i],b=moves[i+1], html=`<div class=\"moveRow ${i+1>=moves.length-1?'current':''}\"><span>${Math.floor(i/2)+1}.</span><span>${esc(w?.san||'')}</span><span>${esc(b?.san||'')}</span></div>`;targets.forEach(h=>h.insertAdjacentHTML('beforeend',html));}\n"
"    if(!moves.length)targets.forEach(h=>h.innerHTML='<div class=\"muted small\">No moves recorded yet.</div>');\n"
"  }\n"
"  function renderCaptured(){\n"
"    const initialCounts={P:8,N:2,B:2,R:2,Q:1,K:1,p:8,n:2,b:2,r:2,q:1,k:1};\n"
"    const counts={P:0,N:0,B:0,R:0,Q:0,K:0,p:0,n:0,b:0,r:0,q:0,k:0};S.engine.board.forEach(p=>{if(p)counts[p]++});\n"
"    const cyanParts=[],grayParts=[];for(const p of ['Q','R','B','N','P']){const n=initialCounts[p]-counts[p];if(n>0)cyanParts.push(pieceFullName(p)+' ×'+n);}for(const p of ['q','r','b','n','p']){const n=initialCounts[p]-counts[p];if(n>0)grayParts.push(pieceFullName(p)+' ×'+n);}\n"
"    $('capturedCyan').textContent=cyanParts.join(', ')||'None';$('capturedGray').textContent=grayParts.join(', ')||'None';\n"
"  }\n"
"  function renderMain(){buildBoard('chessBoard');buildBoard('homeBoard');renderStats();renderHistory();renderCaptured();renderReadablePosition();renderBoardDiagnostics();renderHome();renderReplayControls();}\n"
"  function renderBoardDiagnostics(){const box=$('physicalState');if(!box)return;let msg=S.statusMessage;if(S.mismatch)msg='Board Position Mismatch — follow the highlighted instruction.';box.textContent=msg;}\n"
"\n"
"  function screenSquare(i){\n"
"    if(S.gameStarted && S.engine.result) return;\n"
"    if(S.promotionPending)return;\n"
"    if(S.selected<0){ if(S.engine.board[i] && colorOf(S.engine.board[i])===S.engine.side){S.selected=i;S.legal=S.engine.legalMovesFrom(i).map(m=>m.to);status(`${sqName(i)} selected — legal moves highlighted.`);}}\n"
"    else if(i===S.selected){S.selected=-1;S.legal=[];status('Selection cleared.');}\n"
"    else if(S.legal.includes(i)){tryDoMove(S.selected,i,null,'screen');}\n"
"    else if(S.engine.board[i] && colorOf(S.engine.board[i])===S.engine.side){S.selected=i;S.legal=S.engine.legalMovesFrom(i).map(m=>m.to);status(`${sqName(i)} selected — legal moves highlighted.`);} else {status('Illegal move — destination is not a legal move for the selected piece.','error');}\n"
"    renderAll();sendLEDHint();\n"
"  }\n"
"  function colorOf(p){return p && p===p.toUpperCase()?WHITE:BLACK;}\n"
"\n"
"  function tryDoMove(from,to,promotion=null,source='physical'){\n"
"    if(S.engine.board[to]==='K'||S.engine.board[to]==='k'){status('The king cannot be captured. Check the king and respond legally.','error');pulseWarning(to);return false;}\n"
"    const legal=S.engine.legalMoves().filter(m=>m.from===from&&m.to===to);\n"
"    let move=legal.find(m=>(m.promotion||null)===promotion);\n"
"    if(legal.length>1 && !promotion){\n"
"      S.promotionPending={from,to,source,options:legal}; showPromotion(); return false;\n"
"    }\n"
"    if(!move){\n"
"      status(`Illegal move: ${sqName(from)} → ${sqName(to)} is not legal. ${whyIllegal(from,to)}`,'error');\n"
"      pulseWarning(to); S.mismatch=false; return false;\n"
"    }\n"
"    const movingSide=S.engine.side;\n"
"    const uci=coord(move.from).toLowerCase()+coord(move.to).toLowerCase()+(move.promotion?move.promotion.toLowerCase():'');\n"
"    if(S.currentPuzzle?.uci && uci!==S.currentPuzzle.uci){status('That move is legal, but it is not the solution for this puzzle. Try again.','error');pulseWarning(to);return false;}\n"
"    if(S.currentLesson?.uci && uci!==S.currentLesson.uci){status('That move is legal, but the lesson asks for a specific move. Try again.','error');pulseWarning(to);return false;}\n"
"    S.engine.makeMove(move,true);\n"
"    if(S.currentPuzzle && uci===S.currentPuzzle.uci){S.stats.puzzleSuccess++;S.currentPuzzle.solved=true;localStorage.setItem('smartchess.stats',JSON.stringify(S.stats));status('Puzzle solved — correct move.','success');}\n"
"    if(S.currentLesson?.uci && uci===S.currentLesson.uci){S.currentLesson.solved=true;S.stats.training++;localStorage.setItem('smartchess.stats',JSON.stringify(S.stats));status('Lesson objective complete — correct move.','success');}\n"
"    S.selected=-1;S.legal=[];S.mismatch=false;S.pendingFrom=-1;\n"
"    status(S.engine.result==='checkmate'?'CHECKMATE — game over.':S.engine.result==='check'?'CHECK — the king is under attack.':`${movingSide===WHITE?'Cyan':'Gray'} moved ${move.san}.`,S.engine.result==='check'?'':'success');\n"
"    recordStatsAfterMove();\n"
"    if(S.engine.result && ['checkmate','stalemate','50-move','threefold','insufficient'].includes(S.engine.result)) endGame();\n"
"    renderAll(); sendLEDHint(); saveCurrentGameMaybe();\n"
"    if(S.online.ws && S.online.connected) sendOnlineMove(move);\n"
"    return true;\n"
"  }\n"
"  function whyIllegal(from,to){\n"
"    if(S.engine.inCheck(S.engine.side))return 'Your king is in check, so you must make a legal response.';\n"
"    if(S.engine.board[to] && colorOf(S.engine.board[to])===S.engine.side)return 'You cannot capture your own piece.';\n"
"    return 'The selected piece cannot legally move to that square.';\n"
"  }\n"
"  function pulseWarning(i){\n"
"    const e=[...document.querySelectorAll('.sq')].find(x=>Number(x.dataset.i)===i);if(e){e.classList.add('mismatch');setTimeout(()=>e.classList.remove('mismatch'),700)}\n"
"    fetchBoard('/api/ledmap?m='+encodeURIComponent(''.padEnd(64,'0').substring(0,i)+'8'+''.padEnd(63-i,'0'))).catch(()=>{});\n"
"  }\n"
"  function showPromotion(){\n"
"    const p=document.createElement('div');p.id='promotionModal';p.style.cssText='position:fixed;inset:0;background:rgba(0,0,0,.62);display:grid;place-items:center;z-index:20';\n"
"    const card=document.createElement('div');card.className='card';card.style.width='min(92vw,380px)';card.innerHTML='<h3>Choose promotion</h3><p class=\"muted small\">Select the piece for the pawn promotion.</p><div id=\"promoBtns\" class=\"btnRow\"></div>';p.appendChild(card);document.body.appendChild(p);\n"
"    const names=[['Q','Queen'],['R','Rook'],['B','Bishop'],['N','Knight']]; const box=$('promoBtns'); for(const [x,n] of names){const b=document.createElement('button');b.textContent=n;b.onclick=()=>{const pp=S.promotionPending;S.promotionPending=null;p.remove();tryDoMove(pp.from,pp.to,S.engine.side===WHITE?x:x.toLowerCase(),pp.source);};box.appendChild(b)}\n"
"  }\n"
"\n"
"  async function fetchBoard(path='/api/board'){\n"
"    const base=S.settings.boardUrl.replace(/\\/$/,'');\n"
"    const url=path.startsWith('http')?path:base+path;\n"
"    const r=await fetch(url,{cache:'no-store'}); if(!r.ok)throw new Error('HTTP '+r.status); return await r.json();\n"
"  }\n"
"  async function sendBoard(path){\n"
"    try{\n"
"      const base=S.settings.boardUrl.replace(/\\/$/,'');\n"
"      const url=path.startsWith('http')?path:base+path;\n"
"      const r=await fetch(url,{cache:'no-store'});\n"
"      const t=await r.text();\n"
"      if(!r.ok)throw new Error(t||('HTTP '+r.status));\n"
"      try{return JSON.parse(t);}catch{return t;}\n"
"    }catch(e){status('Board command failed: '+e.message,'error');throw e;}\n"
"  }\n"
"\n"
"  async function pollBoard(){\n"
"    try{\n"
"      const d=await fetchBoard('/api/board?ts='+Date.now()); S.connected=true; S.physical=d; renderTop();\n"
"      const fp=boardFingerprint(d.matrix); if(fp!==S.lastBoardFingerprint){S.lastBoardFingerprint=fp;handlePhysicalChange(d);}\n"
"      // Keep the Home page a live physical+LED mirror, including animation color changes.\n"
"      buildBoard('homeBoard');\n"
"      renderBoardDiagnostics();\n"
"    }catch(e){S.connected=false;renderTop();}\n"
"  }\n"
"\n"
"  function handlePhysicalChange(d){\n"
"    const observed=[]; for(let r=0;r<8;r++)for(let f=0;f<8;f++)if(d.matrix?.[r]?.[f])observed.push(r*8+f);\n"
"    const expected=occFromEngine(); const removed=[],added=[];\n"
"    for(let i=0;i<64;i++){if(expected[i]&&!observed.includes(i))removed.push(i);if(!expected[i]&&observed.includes(i))added.push(i);}\n"
"\n"
"    if(S.pendingFrom<0 && removed.length===1){\n"
"      const from=removed[0]; if(S.engine.board[from] && colorOf(S.engine.board[from])===S.engine.side){S.pendingFrom=from;S.selected=from;S.legal=S.engine.legalMovesFrom(from).map(m=>m.to);status(`Lifted ${sqName(from)}. Place it on one of the highlighted legal squares.`);sendLEDHint();renderAll();return;}\n"
"    }\n"
"    if(S.pendingFrom>=0 && added.length>=1){\n"
"      const from=S.pendingFrom; const candidate=added.find(to=>S.engine.legalMoves().some(m=>m.from===from&&m.to===to));\n"
"      if(candidate!==undefined){tryDoMove(from,candidate,null,'physical');return;}\n"
"      if(removed.includes(from) && added.length===1){status(`Illegal destination ${sqName(added[0])}. Return the piece to ${sqName(from)}.`, 'error');pulseWarning(added[0]);return;}\n"
"    }\n"
"\n"
"    // Castling repair: after the king move the engine expects the rook on its new square.\n"
"    if(S.engine.lastMove?.castle && removed.length===1 && added.length===1){\n"
"      return;\n"
"    }\n"
"    const exact=observed.every(i=>expected[i]) && observed.length===expected.filter(Boolean).length;\n"
"    S.mismatch=!exact;\n"
"    if(S.mismatch && S.engine.moveHistory.length){status('Board Position Mismatch — physical pieces do not match the expected chess position.','error');}\n"
"    renderAll();sendLEDHint();\n"
"  }\n"
"\n"
"  function makeLEDMap(){\n"
"    const m=Array(64).fill('0');\n"
"    // Stationary pieces = green (code 1). Legal destinations = yellow (3/5).\n"
"    S.engine.board.forEach((p,i)=>{ if(p) m[i]='1'; });\n"
"    if(S.legal.length)S.legal.forEach(i=>m[i]=S.engine.board[i]?'5':'3');\n"
"    // Physical lift = blue. A digital selection can use the same blue cue.\n"
"    if(S.pendingFrom>=0)m[S.pendingFrom]='4'; else if(S.selected>=0)m[S.selected]='4';\n"
"    if(S.engine.inCheck(WHITE)){const k=S.engine.findKing(WHITE);if(k>=0)m[k]='8';}\n"
"    if(S.engine.inCheck(BLACK)){const k=S.engine.findKing(BLACK);if(k>=0)m[k]='8';}\n"
"    if(S.mismatch&&S.pendingFrom>=0)m[S.pendingFrom]='8';\n"
"    return m.join('');\n"
"  }\n"
"  function sendLEDHint(){\n"
"    // Do not overwrite a user-selected LED animation while the app is merely mirroring sensors.\n"
"    // Chess-play hints still take ownership when a chess game/puzzle is active.\n"
"    if(!S.gameStarted && S.selected<0 && !S.legal.length) return;\n"
"    if(S.physical?.animation && String(S.physical.animation).toUpperCase()!=='OFF' && !S.gameStarted) return;\n"
"    sendBoard('/api/ledmap?m='+encodeURIComponent(makeLEDMap())).catch(()=>{});\n"
"  }\n"
"  function clearLEDs(){sendBoard('/alloff').catch(()=>{});}\n"
"\n"
"  function recordStatsAfterMove(){\n"
"    // Useful statistics stay descriptive; no rating score is invented here.\n"
"  }\n"
"  function endGame(){S.gameStarted=false;const res=S.engine.result;if(res==='checkmate')S.stats.games++;else S.stats.games++;if(res==='checkmate'){if((S.engine.side===BLACK&&S.settings.cyanIsWhite)||(S.engine.side===WHITE&&!S.settings.cyanIsWhite))S.stats.wins++;else S.stats.losses++;}else S.stats.draws++;localStorage.setItem('smartchess.stats',JSON.stringify(S.stats));}\n"
"  function saveCurrentGameMaybe(){\n"
"    if(!S.activeGameId)S.activeGameId=Date.now();\n"
"    const item={id:S.activeGameId,date:new Date().toISOString(),white:S.settings.cyanIsWhite?'Cyan':'Gray',black:S.settings.cyanIsWhite?'Gray':'Cyan',mode:S.currentPuzzle?'Puzzle':'Local Human vs Human',moves:S.engine.moveHistory.slice(),fen:S.engine.fen(),result:S.engine.result||'in progress'};\n"
"    const idx=S.savedGames.findIndex(g=>g.id===item.id);\n"
"    if(idx>=0)S.savedGames[idx]=item;else S.savedGames=[item,...S.savedGames].slice(0,50);\n"
"    localStorage.setItem('smartchess.games',JSON.stringify(S.savedGames));\n"
"  }\n"
"  function makePGNFromGame(g){\n"
"    if(!g)return '';\n"
"    let text=`[Event \"Smart Chess\"]\\n[White \"${g.white||'Cyan'}\"]\\n[Black \"${g.black||'Gray'}\"]\\n[Date \"${(g.date||new Date().toISOString()).slice(0,10).replaceAll('-','.') }\"]\\n[Result \"${g.result==='checkmate'?'1-0':g.result==='clock'?'1-0':'*'}\"]\\n\\n`;\n"
"    const mv=g.moves||[];for(let i=0;i<mv.length;i++){if(i%2===0)text+=`${Math.floor(i/2)+1}. `;text+=(mv[i].san||'')+' ';}return text.trim()+'\\n';\n"
"  }\n"
"  function downloadText(filename,text,mime='text/plain'){const a=document.createElement('a');a.href=URL.createObjectURL(new Blob([text],{type:mime}));a.download=filename;document.body.appendChild(a);a.click();setTimeout(()=>{URL.revokeObjectURL(a.href);a.remove()},500);}\n"
"  window.exportGame=id=>{const g=S.savedGames.find(x=>x.id===id);if(g)downloadText('smartchess-game-'+id+'.pgn',makePGNFromGame(g),'application/x-chess-pgn');};\n"
"  window.renameGame=id=>{const g=S.savedGames.find(x=>x.id===id);if(!g)return;const name=prompt('Game name',g.name||'Smart Chess Game');if(name){g.name=name;localStorage.setItem('smartchess.games',JSON.stringify(S.savedGames));renderGames();}};\n"
"  function exportCurrentPGN(){saveCurrentGameMaybe();const g=S.savedGames.find(x=>x.id===S.activeGameId);if(g){$('pgnBox').value=makePGNFromGame(g);downloadText('smartchess-current-game.pgn',$('pgnBox').value,'application/x-chess-pgn');}}\n"
"\n"
"  function startLocal(){S.engine.reset(START_FEN);S.selected=-1;S.legal=[];S.pendingFrom=-1;S.currentPuzzle=null;S.currentLesson=null;S.replay=null;S.activeGameId=Date.now();S.gameStarted=true;S.mismatch=false;S.timer.running=S.timer.enabled;S.timer.last=Date.now();status('Local game started. Cyan moves first.');renderAll();sendLEDHint();}\n"
"  function resetBoard(){S.engine.reset(START_FEN);S.selected=-1;S.legal=[];S.pendingFrom=-1;S.currentPuzzle=null;S.currentLesson=null;S.replay=null;S.activeGameId=null;S.gameStarted=false;S.mismatch=false;status('Board reset to the standard starting position.');renderAll();clearLEDs();}\n"
"\n"
"  function renderClocks(){\n"
"    $('clockCyan').textContent=formatClock(S.timer.w);$('clockGray').textContent=formatClock(S.timer.b);\n"
"  }\n"
"  function formatClock(s){s=Math.max(0,Math.floor(s));return String(Math.floor(s/60)).padStart(2,'0')+':'+String(s%60).padStart(2,'0');}\n"
"  function updateClock(){if(!S.timer.enabled||!S.timer.running)return;const now=Date.now(),dt=Math.floor((now-S.timer.last)/1000);if(dt<=0)return;S.timer.last+=dt*1000;if(S.engine.side===WHITE)S.timer.w-=dt;else S.timer.b-=dt;if(S.timer.w<=0||S.timer.b<=0){S.timer.running=false;S.engine.result='clock';status('Time expired.','error');}renderClocks();}\n"
"\n"
"  function renderAll(){renderMain();renderTop();renderLearn();renderPuzzles();renderGames();renderProfile();renderAnalysis();renderSettings();}\n"
"\n"
"  const lessons=[\n"
"    ['Piece movement','Learn how pawns, knights, bishops, rooks, queens and kings move.','Select a piece and use the yellow full-square highlights to inspect legal destinations.',START_FEN],\n"
"    ['Check','A king in check must respond to the attack.','Study the checking line and make a legal response.', '6k1/5Q2/6K1/8/8/8/8/8 b - - 0 1'],\n"
"    ['Checkmate','Deliver checkmate without ever capturing the king.','Find Qg7# on the real board. The engine will reject king captures and accept only the mating move.','7k/5Q2/6K1/8/8/8/8/8 w - - 0 1','f7g7'],\n"
"    ['Castling','Move the king and rook together under the normal king-safety rules.','The rules engine validates both king and rook conditions.',START_FEN],\n"
"    ['En passant','A one-turn-only special pawn capture.','Set up the position and inspect the legal move list.', '8/8/8/3pP3/8/8/8/4K2k w - d6 0 1'],\n"
"    ['Promotion','A pawn reaching the last rank must promote.','Choose queen, rook, bishop or knight when the promotion dialog appears.', '7k/P7/8/8/8/8/6K1/8 w - - 0 1'],\n"
"    ['Opening principles','Develop pieces, contest the center and keep the king safe.','Practice useful developing moves.',START_FEN],\n"
"    ['King safety','Before moving, check whether your king becomes exposed.','The engine rejects self-checking moves.', '4k3/8/8/3q4/8/8/4K3/8 w - - 0 1'],\n"
"    ['Material','Compare the pieces you have with the pieces your opponent has.','Captured pieces are shown by full names.',START_FEN],\n"
"    ['Fork','One move attacks two targets.','Use the board and analysis view to inspect tactical choices.', '6k1/8/8/3N4/4q3/8/8/4K3 w - - 0 1'],\n"
"    ['Pin','A piece is restricted because moving it exposes a more valuable target.','Use the checking line as the constraint.', '4k3/8/8/3b4/8/3R4/8/4K3 w - - 0 1'],\n"
"    ['Skewer','Attack a valuable piece so that a second target is exposed behind it.','Practice identifying aligned targets.', '4k3/4q3/8/8/8/3R4/8/4K3 w - - 0 1'],\n"
"    ['Discovered attack','Move one piece to reveal an attack from another piece.','Look for blocked lines.', '4k3/8/8/3B4/3R4/8/8/4K3 w - - 0 1'],\n"
"    ['Double attack','One move creates two threats.','Compare candidate moves and inspect the resulting attacks.', '6k1/8/4q3/3N4/8/8/8/4K3 w - - 0 1'],\n"
"    ['Basic endgames','Use active kings and passed pawns to convert simplified positions.','Practice a king-and-pawn setup.', '8/8/8/8/8/4K3/4P3/4k3 w - - 0 1'],\n"
"    ['Basic tactics','Checks, captures and threats are a practical search order.','Use the position together with the analysis page.',START_FEN]\n"
"  ];\n"
"  const puzzles=[\n"
"    {title:'Mate in 1 — Edge Box',type:'Mate in 1',difficulty:'Easy',fen:'7k/5Q2/6K1/8/8/8/8/8 w - - 0 1',uci:'f7g7',explain:'Qg7# is mate. The king is never a capture target.'},\n"
"    {title:'Mate in 1 — Corner',type:'Mate in 1',difficulty:'Easy',fen:'7k/6Q1/6K1/8/8/8/8/8 w - - 0 1',uci:'g7h7',explain:'Qh7# is mate because the white king protects h7 and the queen covers the escapes.'},\n"
"    {title:'Mate in 1 — Central net',type:'Mate in 1',difficulty:'Medium',fen:'6k1/5Q2/6K1/8/8/8/8/8 w - - 0 1',uci:'f7g8',explain:'Qg8# is mate in the cornered position.'},\n"
"    {title:'Find the forcing mate',type:'Tactical challenge',difficulty:'Medium',fen:'6k1/5ppp/8/8/8/4Q3/5PPP/6K1 w - - 0 1',uci:'e3e8',explain:'Qe8# is a forcing checkmate. The king is not captured.'}\n"
"  ];\n"
"  function renderLearn(){const g=$('lessonGrid');if(!g)return;g.innerHTML=lessons.map((l,i)=>`<div class=\"lesson\"><div class=\"tile-kicker\">CHESS LESSON ${String(i+1).padStart(2,'0')}</div><h4>${esc(l[0])}</h4><p>${esc(l[1])}</p><div class=\"tile-detail\">${esc(l[2])}</div><button class=\"primary\" onclick=\"window.openLesson(${i})\">OPEN LESSON ON BOARD</button></div>`).join('');}\n"
"  window.openLesson=i=>{const l=lessons[i];S.currentPuzzle=null;S.currentLesson={title:l[0],uci:l[4]||null,solved:false};S.engine.reset(l[3]||START_FEN);S.gameStarted=true;S.selected=-1;S.legal=[];S.pendingFrom=-1;navigate('play');status(l[0]+': '+l[2]);renderAll();sendLEDHint();};\n"
"  function renderPuzzles(){const g=$('puzzleGrid');if(!g)return;g.innerHTML=puzzles.map((p,i)=>`<div class=\"puzzle\"><div class=\"tile-kicker\">CURATED TACTICAL TRAINING ${String(i+1).padStart(2,'0')}</div><h4>${esc(p.title)}</h4><p>${esc(p.type)} - ${esc(p.difficulty)}</p><div class=\"tile-detail\">${esc(p.explain||'Find the legal move that completes the challenge.')}</div><button class=\"primary\" onclick=\"window.startPuzzle(${i})\">SOLVE ON BOARD</button></div>`).join('');}\n"
"  window.startPuzzle=i=>{const p=puzzles[i];S.currentLesson=null;S.engine.reset(p.fen);S.gameStarted=true;S.selected=-1;S.legal=[];S.pendingFrom=-1;S.currentPuzzle=Object.assign({},p,{solved:false});S.stats.puzzles++;localStorage.setItem('smartchess.stats',JSON.stringify(S.stats));navigate('play');status('Puzzle: '+p.title+'. Find the requested move on the board. The king cannot be captured.');renderAll();sendLEDHint();};\n"
"\n"
"  function renderGames(){const g=$('gamesList');if(!g)return;if(!S.savedGames.length){g.innerHTML='<div class=\"muted small\">No saved games yet.</div>';return;}g.innerHTML=S.savedGames.map(x=>`<div class=\"listItem\"><div><b>${esc(x.name||(`${x.white} vs ${x.black}`))}</b><div class=\"meta\">${esc(new Date(x.date).toLocaleString())} • ${esc(x.result)} • ${x.moves?.length||0} plies</div></div><div class=\"btnRow\"><button onclick=\"window.replayGame(${x.id})\">Replay</button><button onclick=\"window.exportGame(${x.id})\">PGN</button><button onclick=\"window.renameGame(${x.id})\">Rename</button><button class=\"danger\" onclick=\"window.deleteGame(${x.id})\">Delete</button></div></div>`).join('');}\n"
"  window.deleteGame=id=>{S.savedGames=S.savedGames.filter(x=>x.id!==id);localStorage.setItem('smartchess.games',JSON.stringify(S.savedGames));renderGames();};\n"
"  window.replayGame=id=>{const x=S.savedGames.find(g=>g.id===id);if(!x)return;S.replay={game:x,index:0};S.engine.reset();navigate('play');status('Replay ready. Use the replay controls in the Play page.');renderReplayControls();renderAll();};\n"
"  function renderReplayControls(){const box=$('replayControls');if(!box)return;box.style.display=S.replay?'flex':'none';}\n"
"  function replayStep(delta){if(!S.replay)return;const target=Math.max(0,Math.min(S.replay.game.moves.length,S.replay.index+delta));S.engine.reset();for(let i=0;i<target;i++){const m=S.replay.game.moves[i];S.engine.playUci(coord(m.from).toLowerCase()+coord(m.to).toLowerCase()+(m.promotion?m.promotion.toLowerCase():''));}S.replay.index=target;status(`Replay ${target}/${S.replay.game.moves.length}`);renderAll();}\n"
"\n"
"  function renderAnalysis(){const box=$('analysisBox');if(!box)return; if(!S.engine.moveHistory.length){box.innerHTML='<div class=\"muted small\">Play a game first. Analysis here uses the rule engine and descriptive material/check signals; it does not pretend to be AI evaluation.</div>';return;} const rows=[]; for(let i=0;i<S.engine.moveHistory.length;i++){const m=S.engine.moveHistory[i];let note='Normal move.';if(m.checkmate)note='Checkmate.';else if(m.check)note='Check — the opponent must respond.';if(m.capture)note+=' Capture.';rows.push(`<div class=\"listItem\"><div><b>${i+1}. ${esc(m.san)}</b><div class=\"meta\">${esc(note)}</div></div></div>`);}box.innerHTML=rows.join('');}\n"
"  function renderProfile(){const s=S.stats;$('profileStats').innerHTML=`<div class=\"stat\"><b>${s.games}</b><span>Games</span></div><div class=\"stat\"><b>${s.wins}</b><span>Wins</span></div><div class=\"stat\"><b>${s.losses}</b><span>Losses</span></div><div class=\"stat\"><b>${s.draws}</b><span>Draws</span></div><div class=\"stat\"><b>${s.puzzles}</b><span>Puzzles opened</span></div><div class=\"stat\"><b>${s.puzzleSuccess}</b><span>Puzzles solved</span></div><div class=\"stat\"><b>${s.training}</b><span>Training sessions</span></div>`;}\n"
"  function renderSettings(){\n"
"    if(!$('boardUrl'))return; $('boardUrl').value=S.settings.boardUrl;$('assistance').value=S.settings.assistance;$('brightness').value=S.settings.brightness;$('brightnessVal').textContent=S.settings.brightness;$('sound').checked=S.settings.sound;$('language').value=S.settings.language;$('theme').value=localStorage.getItem('smartchess.theme')||'prime';$('cyanWhite').value=String(S.settings.cyanIsWhite);$('aiDifficulty').value=S.settings.aiDifficulty;$('whiteName').value=S.settings.whiteName;$('blackName').value=S.settings.blackName;\n"
"  }\n"
"\n"
"  async function checkBoard(){try{const d=await fetchBoard('/api/board?ts='+Date.now());S.connected=true;status(`Connected to ${d.chip||'Smart Chess board'} at ${S.settings.boardUrl}.`,'success');}catch(e){S.connected=false;status('Board connection failed. Check the ESP32 IP and Wi-Fi.','error')}renderTop();}\n"
"  function saveSettings(){S.settings.boardUrl=$('boardUrl').value.trim()||location.origin;S.settings.assistance=$('assistance').value;S.settings.brightness=Number($('brightness').value);S.settings.sound=$('sound').checked;S.settings.language=$('language').value;localStorage.setItem('smartchess.theme',$('theme').value);S.settings.cyanIsWhite=$('cyanWhite').value==='true';S.settings.aiDifficulty=$('aiDifficulty').value;S.settings.whiteName=$('whiteName').value||'Cyan';S.settings.blackName=$('blackName').value||'Gray';Object.entries(S.settings).forEach(([k,v])=>localStorage.setItem('smartchess.'+k,String(v)));status('Settings saved.','success');sendBoard('/brightness?value='+S.settings.brightness).catch(()=>{});}\n"
"\n"
"  async function connectOnline(){const url=$('wsUrl').value.trim(),room=$('roomCode').value.trim().toUpperCase();if(!url||!room){status('Enter a WebSocket URL and room code.','error');return;}try{const ws=new WebSocket(url);S.online.ws=ws;ws.onopen=()=>{S.online.connected=true;ws.send(JSON.stringify({type:'join',room,name:$('onlineName').value||'Player'}));status('Online room connected.','success')};ws.onmessage=e=>handleOnline(JSON.parse(e.data));ws.onclose=()=>{S.online.connected=false;status('Online room disconnected.','error')};ws.onerror=()=>status('Online connection error.','error');S.online.room=room;}catch(e){status(e.message,'error')}}\n"
"  function sendOnlineMove(move){if(!S.online.ws||S.online.ws.readyState!==1)return;S.online.ws.send(JSON.stringify({type:'move',room:S.online.room,uci:coord(move.from).toLowerCase()+coord(move.to).toLowerCase()+(move.promotion?move.promotion.toLowerCase():''),fen:S.engine.fen()}));}\n"
"  function handleOnline(msg){if(msg.type==='state'&&msg.fen){S.engine.reset(msg.fen);status('Opponent move received. Board synchronized.');renderAll();sendLEDHint();}else if(msg.type==='error')status(msg.message,'error');else if(msg.type==='joined')status(`Joined room ${msg.room}.`,'success');}\n"
"\n"
"  function externalHubBase(){return (localStorage.getItem('sc.hubUrl')||location.origin).replace(/\\/$/,'');}\n"
"  async function chessComPlayer(){const u=$('chessUser').value.trim();if(!u)return;try{const d=await fetch(externalHubBase()+'/api/chesscom/player/'+encodeURIComponent(u)).then(async r=>{const t=await r.text();if(!r.ok)throw new Error(t||('HTTP '+r.status));return JSON.parse(t);});S.chessCom.player=d; const out=$('chessResult');if(out)out.textContent=JSON.stringify(d,null,2);}catch(e){const out=$('chessResult');if(out)out.textContent='Chess.com lookup failed. Use the configured Smart Chess Hub. '+e.message;}}\n"
"  async function chessComPgn(){const u=$('chessUser').value.trim(),y=$('pgnYear').value,m=String($('pgnMonth').value).padStart(2,'0');if(!u)return;try{const r=await fetch(externalHubBase()+`/api/chesscom/games/${encodeURIComponent(u)}/${y}/${m}`);if(!r.ok)throw new Error('HTTP '+r.status);S.chessCom.pgn=await r.text();$('pgnBox').value=S.chessCom.pgn;}catch(e){$('pgnBox').value='Chess.com public game archive import failed. '+e.message;}}\n"
"\n"
"  function bind(){\n"
"    document.querySelectorAll('.nav button').forEach(b=>b.onclick=()=>navigate(b.dataset.page));\n"
"    $('startGame').onclick=startLocal;$('resetGame').onclick=resetBoard;$('checkBoard').onclick=checkBoard;$('saveSettings').onclick=saveSettings;$('clearLEDs').onclick=clearLEDs;$('sendHint').onclick=sendLEDHint;$('exportCurrent').onclick=exportCurrentPGN;\n"
"    $('enableTimer').onchange=e=>{S.timer.enabled=e.target.checked;S.timer.running=S.timer.enabled;S.timer.last=Date.now();};\n"
"    $('replayBack').onclick=()=>replayStep(-1);$('replayForward').onclick=()=>replayStep(1);$('replayStart').onclick=()=>{if(S.replay){S.replay.index=0;replayStep(0)}};$('replayEnd').onclick=()=>{if(S.replay){S.replay.index=S.replay.game.moves.length;replayStep(0)}};\n"
"    $('connectOnline').onclick=connectOnline;$('chessLookup').onclick=chessComPlayer;$('chessImport').onclick=chessComPgn;\n"
"    $('brightness').oninput=e=>$('brightnessVal').textContent=e.target.value;\n"
"    setInterval(pollBoard,500);setInterval(updateClock,250);pollBoard();\n"
"  }\n"
"\n"
"  function renderHome(){const e=$('homeBoardStatus');if(e)e.textContent=S.connected?'Physical board connected.':'Connect the Smart Chess board from Settings.';}\n"
"\n"
"  window.addEventListener('load',()=>{bind();renderAll();renderHome();});\n"
"})();\n"
"\n"
"</script>\n"
"<script>\n"
"(() => {\n"
"  'use strict';\n"
"\n"
"  const $ = (id) => document.getElementById(id);\n"
"  const A = {\n"
"    board: localStorage.getItem('sc.boardUrl') || localStorage.getItem('smartchess.boardUrl') || location.origin,\n"
"    anim: 'OFF',\n"
"    palette: [],\n"
"    custom: null,\n"
"    last: null,\n"
"    connected: false\n"
"  };\n"
"\n"
"  const defaults = ['#00D2FF','#FF50B4','#FFD230','#50FF78','#8A6CFF','#FF7C2A','#FFFFFF','#00FFB2'];\n"
"\n"
"  function esc(s) {\n"
"    return String(s ?? '').replace(/[&<>\"']/g, m => ({'&':'&amp;','<':'&lt;','>':'&gt;','\"':'&quot;',\"'\":'&#39;'}[m]));\n"
"  }\n"
"  function boardURL() { return (A.board || location.origin).replace(/\\/$/, ''); }\n"
"  function hubURL() {\n"
"    return (localStorage.getItem('sc.hubUrl') || $('hubUrl')?.value || location.origin).replace(/\\/$/, '');\n"
"  }\n"
"  function aiURL() {\n"
"    return (localStorage.getItem('sc.aiUrl') || $('aiUrl')?.value || 'http://localhost:8080').replace(/\\/$/, '');\n"
"  }\n"
"  async function text(url, opts = {}) {\n"
"    const r = await fetch(url, {cache:'no-store', ...opts});\n"
"    const t = await r.text();\n"
"    if (!r.ok) throw new Error(t || ('HTTP ' + r.status));\n"
"    return t;\n"
"  }\n"
"  async function json(url, opts = {}) {\n"
"    const r = await fetch(url, {cache:'no-store', ...opts});\n"
"    const t = await r.text();\n"
"    if (!r.ok) throw new Error(t || ('HTTP ' + r.status));\n"
"    try { return JSON.parse(t); } catch { throw new Error('Invalid JSON from ' + url + ': ' + t.slice(0, 160)); }\n"
"  }\n"
"  function toast(msg, kind = 'info') {\n"
"    const t = $('toast'); if (!t) return;\n"
"    t.textContent = msg; t.className = 'toast show ' + kind;\n"
"    clearTimeout(window.__v10toast); window.__v10toast = setTimeout(() => t.className = 'toast', 2800);\n"
"  }\n"
"  function set(id, value) { const e = $(id); if (e) e.textContent = value; }\n"
"  function formatUptime(ms){\n"
"    const sec=Math.max(0,Math.floor(Number(ms||0)/1000));\n"
"    const d=Math.floor(sec/86400), h=Math.floor(sec%86400/3600), m=Math.floor(sec%3600/60), s=sec%60;\n"
"    return (d?d+'d ':'')+String(h).padStart(2,'0')+':'+String(m).padStart(2,'0')+':'+String(s).padStart(2,'0');\n"
"  }\n"
"  function setMessage(id, value, kind='info') { const e=$(id); if(e){e.textContent=value;e.className='message '+kind;} }\n"
"\n"
"  function boardLedIndex(r, c) {\n"
"    const ledRow = 7 - r;\n"
"    return (ledRow % 2 === 0) ? ledRow * 8 + c : ledRow * 8 + (7 - c);\n"
"  }\n"
"\n"
"  function colorForCode(code, d) {\n"
"    if (code === 1) return d?.gameP1Color || '#00D2FF';\n"
"    if (code === 2) return d?.gameP2Color || '#FF50B4';\n"
"    if (code === 3) return d?.gameTargetColor || '#F2CA5C';\n"
"    if (code === 8) return d?.gameWarningColor || '#FF0000';\n"
"    if (code === 10) return d?.gameGuideColor || '#005ABF';\n"
"    if (code === 4) return '#00E6FF';\n"
"    if (code === 5) return '#FF5B66';\n"
"    if (code === 6) return '#F4CB68';\n"
"    if (code === 7) return '#AB8CFF';\n"
"    return '#05090C';\n"
"  }\n"
"\n"
"  function drawLedPreview(d = A.last || {}) {\n"
"    const el = $('ledBoard'); if (!el) return;\n"
"    const colors = Array.isArray(d.ledColors) && d.ledColors.length === 64 ? d.ledColors : null;\n"
"    const grid = Array.isArray(d.gameGrid) && d.gameGrid.length === 64 ? d.gameGrid : Array(64).fill(0);\n"
"    el.innerHTML = '';\n"
"    for (let r = 7; r >= 0; r--) {\n"
"      for (let c = 0; c < 8; c++) {\n"
"        const i = r * 8 + c;\n"
"        const cell = document.createElement('button');\n"
"        cell.type = 'button';\n"
"        cell.className = 'led-cell';\n"
"        const activeColor = colors ? colors[i] : colorForCode(grid[i], d);\n"
"        cell.style.background = activeColor || '#05090C';\n"
"        if (activeColor && activeColor !== '#05090C') cell.style.boxShadow = `0 0 16px ${activeColor}, inset 0 0 10px rgba(255,255,255,.14)`;\n"
"        cell.title = String.fromCharCode(65+c) + (r+1) + ' • LED ' + boardLedIndex(r,c);\n"
"        cell.onclick = () => manualLED(r,c);\n"
"        el.appendChild(cell);\n"
"      }\n"
"    }\n"
"  }\n"
"\n"
"  async function manualLED(r,c) {\n"
"    const color = ($('manualColor')?.value || '#27D8E9').toUpperCase();\n"
"    try {\n"
"      await text(boardURL() + '/led?index=' + boardLedIndex(r,c) + '&color=' + encodeURIComponent(color));\n"
"      toast(`LED ${String.fromCharCode(65+c)}${r+1} → ${color}`, 'success');\n"
"      await poll();\n"
"    } catch (e) { toast('LED command failed: ' + e.message, 'error'); }\n"
"  }\n"
"\n"
"  function syncTop(d) {\n"
"    const boardIsLive=!!(d&&d.hardwareOnline===true);\n"
"    A.last=d; A.connected=boardIsLive; S.connected=boardIsLive; S.physical=d; renderTop();\n"
"    set('mBoard',boardIsLive?'ONLINE':'OFFLINE');set('mActive',d.active??0);set('mTurn',d.turn||$('turn')?.textContent||'Cyan');set('mState',d.gameStatus||'Ready');set('mAnim',d.animation||'OFF');set('mBright',d.brightness??'—');set('mRaw',d.rawActiveCells??d.active??0);set('mHeap',d.heap?Math.round(d.heap/1024)+' KB':'--');set('mCpu',d.cpuMHz?d.cpuMHz+' MHz':'--');\n"
"    set('topBoardText',boardIsLive?'ONLINE':'OFFLINE');set('homeAnimState',d.animation||'OFF');set('animationStateChip',d.animation||'OFF');set('homeSSID',d.ssid||'--');set('homeWifi',d.wifiMode||'--');set('homeUptime',d.uptimeMs!=null?formatUptime(d.uptimeMs):'--');set('homeRawCells',d.rawActiveCells??d.active??0);set('homeLEDMode',d.animation||'OFF');set('homeGameMode',d.gameMode||'NONE');set('homeGameScore',d.gameScore??0);set('homeGameMoves',d.gameMoves??0);set('ledCenterStatus',d.animation&&d.animation!=='OFF'?('RUNNING • '+d.animation):'LIVE MIRROR');set('homeSignal','LINK STABLE');set('topAiText',localStorage.getItem('sc.aiUrl')?'CONFIGURED':'NOT SET');set('latestEvent',d.lastEvent||'Waiting for switch activity');set('latestSquare',d.lastSquare||'--');\n"
"    const legacyTurn=$('turn')?.textContent||'Cyan';set('playTurn',legacyTurn);set('playResult',$('gameState')?.textContent||d.gameStatus||'playing');set('playHalf',$('halfmove')?.textContent||'0');renderReadablePosition();\n"
"    const modeChip=$('playModeChip');if(modeChip)modeChip.textContent=(d.gameActive?(d.gameMode||'GAME'):'LOCAL').toUpperCase();set('rawRowMask',d.rawRowMask??0);set('rawColMask',d.rawColMask??0);set('multiRaw',d.multipleRawDetected?'YES':'NO');set('diagMode',d.diagnosticMessage||'Ready');set('deviceIP',d.ip||'--');set('setSensitivity',d.sensitivity??'--');set('setSettle',d.settleUs!==undefined?d.settleUs+' us':'--');set('setCalibration',(d.calibratedSquares??0)+'/64');set('setMapping',d.colText||'--');\n"
"    const chip=$('homeStateChip');if(chip)chip.textContent=d.gameActive?(d.gameMode||'GAME').toUpperCase():(d.animation||'READY');set('ledModeChip',d.animation||'OFF');const live=$('liveText');if(live){live.textContent=boardIsLive?'BOARD CONNECTED':'BOARD OFFLINE';live.className=boardIsLive?'live-ok':'live-off';}['liveDot','sideDot'].forEach(id=>{const e=$(id);if(e)e.className='dot '+(boardIsLive?'on':'bad');});const sb=$('sideBoardText');if(sb){sb.textContent=boardIsLive?'Board connected':'Board offline';sb.className=boardIsLive?'live-ok':'live-off';}\n"
"    const msg=d.gameActive?('Game: '+(d.gameMode||'Game')+' • '+(d.gameStatus||'active')):('Animation: '+(d.animation||'OFF')+' • Sensors: '+(d.active??0));setMessage('homeMessage',msg,d.multipleRawDetected?'warning':'info');const dm=$('diagMessage');if(dm){dm.textContent=d.multipleRawDetected?'Multiple raw activations detected — inspect wiring/ghosting.':(d.diagnosticMessage||'Matrix reading normally.');dm.className='message '+(d.multipleRawDetected?'warning':'success');}updateColors(d);drawLedPreview(d);updateTelemetry(d);buildBoard('homeBoard');\n"
"  }\n"
"  function updateColors(d) {\n"
"    [['trackingColor',d.trackingColor],['p1Color',d.gameP1Color],['p2Color',d.gameP2Color],['guideColor',d.gameGuideColor],['legalColor',d.gameLegalColor],['liftColor',d.gameLiftColor],['warningColor',d.gameWarningColor]].forEach(([id,v])=>{const e=$(id);if(e&&v)e.value=v;});\n"
"    if ($('brightRange') && d.brightness !== undefined) { $('brightRange').value=d.brightness;set('brightLabel',d.brightness); }\n"
"    if ($('speedRange') && d.speed !== undefined) { $('speedRange').value=d.speed;set('speedLabel',d.speed); }\n"
"    setAnimButtons(d.animation || 'OFF');\n"
"  }\n"
"\n"
"  function setAnimButtons(current) {\n"
"    const key = String(current).toLowerCase().replace(/[ _-]/g,'');\n"
"    document.querySelectorAll('.anim-btn').forEach(b=>b.classList.toggle('active',String(b.dataset.anim).toLowerCase().replace(/[ _-]/g,'')===key));\n"
"  }\n"
"\n"
"  async function poll() {\n"
"    try {\n"
"      const d = await json(boardURL() + '/api/board?ts=' + Date.now());\n"
"      syncTop(d);\n"
"      return d;\n"
"    } catch (e) {\n"
"      A.connected = false; S.connected = false; renderTop();\n"
"      set('mBoard','OFFLINE');\n"
"      ['liveDot','sideDot'].forEach(id=>{const el=$(id);if(el)el.className='dot bad';});\n"
"      set('liveText','BOARD OFFLINE');\n"
"      set('sideBoardText','Board offline');\n"
"      setMessage('homeMessage','Board unreachable. Check Wi-Fi and the Board URL in Connections / Settings.','error');\n"
"      return null;\n"
"    }\n"
"  }\n"
"\n"
"  async function sendCmd(path, opts={}) {\n"
"    try {\n"
"      const t = await text(boardURL() + path, opts);\n"
"      toast(t || 'Command sent.', 'success');\n"
"      await poll();\n"
"      return t;\n"
"    } catch (e) {\n"
"      toast('Board command failed: ' + e.message, 'error');\n"
"      throw e;\n"
"    }\n"
"  }\n"
"\n"
"  async function selectAnim(mode) {\n"
"    try {\n"
"      await text(boardURL() + '/animation?mode=' + encodeURIComponent(mode));\n"
"      toast(mode === 'none' ? 'Animations stopped.' : ('Animation started: ' + mode), 'success');\n"
"      await poll();\n"
"      if (mode !== 'none') await loadPalette();\n"
"    } catch (e) { toast('Animation failed: ' + e.message, 'error'); }\n"
"  }\n"
"\n"
"  async function loadPalette() {\n"
"    try {\n"
"      const d = A.last || await json(boardURL() + '/api/board');\n"
"      A.anim = d.animation || 'OFF';\n"
"      A.palette = Array.isArray(d.animationColors) ? d.animationColors.slice() : [];\n"
"      const title=$('paletteTitle'); if(title)title.textContent=A.anim;\n"
"      const help=$('paletteHelp'); if(help)help.textContent=A.palette.length ? `This animation exposes exactly ${A.palette.length} saved color${A.palette.length===1?'':'s'}. Changes are independent per animation.` : 'Start a built-in animation first.';\n"
"      const box=$('paletteFields'); if(!box)return;\n"
"      box.innerHTML='';\n"
"      A.palette.forEach((c,i)=>{\n"
"        const w=document.createElement('label'); w.className='color-item';\n"
"        w.innerHTML=`<span>COLOR ${i+1}</span><input type=\"color\" value=\"${c}\" data-pidx=\"${i}\">`;\n"
"        const inp=w.querySelector('input');inp.oninput=e=>{A.palette[i]=e.target.value.toUpperCase();w.style.setProperty('--swatch',e.target.value);};\n"
"        box.appendChild(w);\n"
"      });\n"
"      setAnimButtons(A.anim);\n"
"    } catch (e) { toast('Palette read failed: ' + e.message, 'error'); }\n"
"  }\n"
"\n"
"  async function savePalette() {\n"
"    const inputs=[...document.querySelectorAll('[data-pidx]')];\n"
"    if(!inputs.length){toast('Select an animation first.','warning');return;}\n"
"    const colors=inputs.map(x=>x.value.toUpperCase()).join(',');\n"
"    try{await text(boardURL()+'/animation/colors?colors='+encodeURIComponent(colors)+'&save=1');toast('Palette saved to ESP32 flash.','success');await poll();}\n"
"    catch(e){toast('Palette save failed: '+e.message,'error');}\n"
"  }\n"
"\n"
"  async function savePieces(){\n"
"    const q=new URLSearchParams({tracking:$('trackingColor').value,p1:$('p1Color').value,p2:$('p2Color').value,guide:$('guideColor').value,legal:$('legalColor').value,lift:$('liftColor').value,warning:$('warningColor').value,save:'1'});\n"
"    try{await text(boardURL()+'/piece/colors?'+q.toString());toast('Piece, move-guide and warning colors saved.','success');await poll();}\n"
"    catch(e){toast('Piece color save failed: '+e.message,'error');}\n"
"  }\n"
"\n"
"  function ensureCustom(){\n"
"    if(!A.custom)A.custom={frames:1,colors:4,delay:120,repeatSeconds:0,frame:0,paint:1,colorsList:[],framesData:[Array(64).fill(0)],name:'My Animation'};return A.custom;\n"
"  }\n"
"  async function loadCustomSlots(){\n"
"    try{const d=await json(boardURL()+'/custom/slots');const sel=$('customSlot');if(!sel)return;sel.innerHTML='';(d.slots||[]).forEach((x,i)=>{const o=document.createElement('option');o.value=i;o.textContent=x.used?x.name:('Slot '+(i+1));sel.appendChild(o)});const active=Number(d.activeSlot||0);sel.value=String(active);const name=$('customName');if(name)name.value=d.activeName||'My Animation';}catch(e){}\n"
"  }\n"
"  async function loadCustom(){\n"
"    try{const d=await json(boardURL()+'/custom');const c=ensureCustom();c.frames=d.frameCount||1;c.colors=d.colorCount||4;c.delay=d.frameDelay||120;c.repeatSeconds=d.repeatSeconds||0;c.name=d.activeName||'My Animation';c.colorsList=(d.colors||[]).slice(0,c.colors);while(c.colorsList.length<c.colors)c.colorsList.push(defaults[c.colorsList.length]||'#FFFFFF');c.framesData=(d.frames||[]).slice(0,c.frames).map(s=>String(s).padEnd(64,'0').slice(0,64).split('').map(Number));while(c.framesData.length<c.frames)c.framesData.push(Array(64).fill(0));c.frame=0;c.paint=1;$('customFrames').value=c.frames;$('customColors').value=c.colors;$('customDelay').value=c.delay;$('customRepeat').value=c.repeatSeconds||0;if($('customName'))$('customName').value=c.name;renderCustom();await loadCustomSlots();}catch(e){toast('Custom animation read failed: '+e.message,'error');}\n"
"  }\n"
"  async function loadCustomSlot(){const slot=Number($('customSlot')?.value||0);try{const d=await json(boardURL()+'/custom/slot/load?slot='+slot);const c=ensureCustom();c.frames=d.frameCount||1;c.colors=d.colorCount||4;c.delay=d.frameDelay||120;c.repeatSeconds=d.repeatSeconds||0;c.name=d.name||('Slot '+(slot+1));c.colorsList=(d.colors||[]).slice(0,c.colors);while(c.colorsList.length<c.colors)c.colorsList.push(defaults[c.colorsList.length]||'#FFFFFF');c.framesData=(d.frames||[]).slice(0,c.frames).map(s=>String(s).padEnd(64,'0').slice(0,64).split('').map(Number));while(c.framesData.length<c.frames)c.framesData.push(Array(64).fill(0));c.frame=0;c.paint=1;$('customFrames').value=c.frames;$('customColors').value=c.colors;$('customDelay').value=c.delay;$('customRepeat').value=c.repeatSeconds||0;if($('customName'))$('customName').value=c.name;renderCustom();set('customStatus','Loaded '+c.name+' from ESP32 flash.');await selectAnim('custom');}catch(e){set('customStatus','Slot '+(slot+1)+' is empty or unavailable. Build it and save.');renderCustom();}}\n"
"  function renderCustom(){\n"
"    const c=ensureCustom();$('customFrames').value=c.frames;$('customColors').value=c.colors;$('customDelay').value=c.delay;$('customRepeat').value=c.repeatSeconds||0;if($('customName'))$('customName').value=c.name||'My Animation';\n"
"    const p=$('customPalette');p.innerHTML='';for(let i=0;i<c.colors;i++){const w=document.createElement('label');w.className='custom-color';const col=c.colorsList[i]||defaults[i]||'#FFFFFF';c.colorsList[i]=col;w.innerHTML=`<small>COLOR ${i+1}</small><input type=\"color\" value=\"${col}\">`;const inp=w.querySelector('input');inp.oninput=e=>{c.colorsList[i]=e.target.value.toUpperCase();renderCustom();};inp.onclick=()=>{c.paint=i+1;renderCustom();};w.onclick=e=>{if(e.target===inp)return;c.paint=i+1;renderCustom();};p.appendChild(w);}\n"
"    const tools=$('paintTools');tools.innerHTML='<span class=\"muted\">Selected:</span>';for(let i=1;i<=c.colors;i++){const b=document.createElement('button');b.className='paint-swatch '+(c.paint===i?'active':'');b.textContent='COLOR '+i;b.style.background=c.colorsList[i-1];b.style.color='#061016';b.onclick=()=>{c.paint=i;renderCustom();};tools.appendChild(b);}const er=document.createElement('button');er.textContent='ERASER';er.className=c.paint===0?'active':'';er.onclick=()=>{c.paint=0;renderCustom();};tools.appendChild(er);\n"
"    const tabs=$('frameTabs');tabs.innerHTML='';for(let i=0;i<c.frames;i++){const b=document.createElement('button');b.textContent='FRAME '+(i+1);b.className=i===c.frame?'primary':'';b.onclick=()=>{c.frame=i;renderCustom();};tabs.appendChild(b);}\n"
"    const paint=$('customPaint');paint.innerHTML='';const fr=c.framesData[c.frame]||Array(64).fill(0);for(let r=7;r>=0;r--)for(let col=0;col<8;col++){const i=r*8+col,x=document.createElement('button');x.type='button';x.className='paint-cell';const v=fr[i]||0;x.style.background=v?(c.colorsList[v-1]||'#FFFFFF'):'#05090C';x.title=String.fromCharCode(65+col)+(r+1);if(v===c.paint&&c.paint)x.classList.add('sel');x.onclick=()=>{fr[i]=c.paint;renderCustom();};paint.appendChild(x);}set('customStatus',`Frame ${c.frame+1}/${c.frames} • ${c.colors} colors • ${c.delay} ms • ${c.repeatSeconds?c.repeatSeconds+' s repeat':'continuous'} • ${c.paint===0?'eraser':'color '+c.paint}`);\n"
"  }\n"
"  function repeatCurrentFrame(){const c=ensureCustom();if(c.frame>=c.frames-1){toast('Already on the last frame.','warning');return;}const src=c.framesData[c.frame].slice();for(let i=c.frame+1;i<c.frames;i++)c.framesData[i]=src.slice();renderCustom();set('customStatus','Frame '+(c.frame+1)+' repeated through the remaining frames.');}\n"
"  async function saveCustomAnimation(){const c=ensureCustom();c.frames=Math.max(1,Math.min(8,Number($('customFrames').value)||1));c.colors=Math.max(1,Math.min(8,Number($('customColors').value)||1));c.delay=Math.max(20,Math.min(2000,Number($('customDelay').value)||120));c.repeatSeconds=Math.max(0,Math.min(600,Number($('customRepeat').value)||0));c.name=(($('customName').value||'My Animation').trim().slice(0,18)||'My Animation');while(c.colorsList.length<c.colors)c.colorsList.push(defaults[c.colorsList.length]||'#FFFFFF');while(c.framesData.length<c.frames)c.framesData.push(Array(64).fill(0));const colors=c.colorsList.slice(0,c.colors).join(',');const frames=c.framesData.slice(0,c.frames).map(fr=>fr.join(''));const slot=Number($('customSlot').value||0);const body=[slot,c.name.replaceAll('|',' '),c.repeatSeconds,c.frames,c.delay,c.colors,colors,...frames].join('|');try{const t=await text(boardURL()+'/custom/slot/save',{method:'POST',headers:{'Content-Type':'text/plain'},body});set('customStatus',t);toast('Saved '+c.name+' to Slot '+(slot+1)+'.','success');await loadCustomSlot();}catch(e){toast('Custom save failed: '+e.message,'error');}}\n"
"  async function deleteCustomAnimation(){const slot=Number($('customSlot').value||0);if(!confirm('Delete the saved animation in this slot?'))return;try{const t=await text(boardURL()+'/custom/slot/delete?slot='+slot);set('customStatus',t);await loadCustomSlots();await loadCustom();}catch(e){toast('Delete failed: '+e.message,'error');}}\n"
"  function clearFrame(){const c=ensureCustom();c.framesData[c.frame]=Array(64).fill(0);renderCustom();}\n"
"\n"
"  async function saveBoard(){\n"
"    A.board=$('boardUrl').value.trim()||location.origin;\n"
"    localStorage.setItem('sc.boardUrl',A.board);localStorage.setItem('smartchess.boardUrl',A.board);\n"
"    toast('Board URL saved.','success');await poll();\n"
"  }\n"
"  async function testBoard(){\n"
"    A.board=$('boardUrl').value.trim()||location.origin;localStorage.setItem('sc.boardUrl',A.board);localStorage.setItem('smartchess.boardUrl',A.board);\n"
"    try{const d=await json(boardURL()+'/api/board');toast('Board online: '+(d.chip||'Smart Chess'), 'success');syncTop(d);}catch(e){toast('Board connection failed: '+e.message,'error');}\n"
"  }\n"
"  async function hardSave(){try{await sendCmd('/settings/save');set('saveMessage','Hardware state saved to ESP32 flash.');}catch(e){}}\n"
"\n"
"  async function aiStatus(){\n"
"    const url=aiURL();\n"
"    localStorage.setItem('sc.aiUrl',url);\n"
"    try{const d=await json(url+'/api/ai/status');\n"
"      $('aiStatusBox').textContent=d.message||JSON.stringify(d,null,2);$('aiStatusBox').className='message '+(d.available?'success':'warning');\n"
"      if($('aiConnectionData'))$('aiConnectionData').textContent=JSON.stringify(d,null,2);\n"
"      if($('aiBadge'))$('aiBadge').textContent=d.available?'AI CONNECTED':'AI NOT CONNECTED';\n"
"      set('topAiText',d.available?'READY':(d.configured?'CONFIGURED':'NOT SET'));\n"
"      toast(d.available?'Real Stockfish adapter is connected.':'AI server reached, but Stockfish is not configured.','success');\n"
"      return d;\n"
"    }catch(e){\n"
"      $('aiStatusBox').textContent='AI server not reachable. Start the Smart Chess Node server and configure STOCKFISH_PATH.';$('aiStatusBox').className='message error';\n"
"      if($('aiConnectionData'))$('aiConnectionData').textContent='Connection error: '+e.message;\n"
"      if($('aiBadge'))$('aiBadge').textContent='AI NOT CONNECTED';\n"
"    }\n"
"  }\n"
"  async function aiAnalyze(){\n"
"    try{\n"
"      const fen=($('playFen')?.textContent||$('fenBox')?.textContent||'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1').trim();\n"
"      const r=await fetch(aiURL()+'/api/ai/analyze',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({fen,depth:Number($('aiDepth')?.value||16)})});\n"
"      const t=await r.text();if(!r.ok)throw new Error(t||('HTTP '+r.status));let d;try{d=JSON.parse(t)}catch{d={raw:t}};\n"
"      $('aiOutput').textContent=JSON.stringify({fen,bestmove:d.bestmove||null,depth:d.depth||null,available:d.available,engine:d.message||'',raw:d.raw||d},null,2);\n"
"      toast(d.bestmove?'Best move received from Stockfish.':'AI returned no best move.','success');\n"
"    }catch(e){$('aiOutput').textContent='AI unavailable: '+e.message;toast('AI analysis failed.','error');}\n"
"  }\n"
"\n"
"  async function hubHealth(){\n"
"    const u=hubURL(); localStorage.setItem('sc.hubUrl',u);\n"
"    if($('hubUrl'))$('hubUrl').value=u;\n"
"    try{const d=await json(u+'/api/health');setMessage('hubState',`Hub online • board proxy ${d.boardUrl||'configured'} • rooms ${d.onlineRooms??0}`,'success');return d;}\n"
"    catch(e){setMessage('hubState','Smart Chess Hub unavailable: '+e.message,'error');}\n"
"  }\n"
"\n"
"  async function chessStatus(){\n"
"    const u=hubURL();localStorage.setItem('sc.hubUrl',u);\n"
"    try{const d=await json(u+'/api/chesscom/status');$('chessResult').textContent=JSON.stringify(d,null,2);$('chessResult').className='connection-data';return d;}\n"
"    catch(e){$('chessResult').textContent='Chess.com connector unavailable: '+e.message;return null;}\n"
"  }\n"
"  async function chessLookup(){\n"
"    const u=hubURL(),name=$('chessUser').value.trim();if(!name)return;\n"
"    try{const d=await json(u+'/api/chesscom/player/'+encodeURIComponent(name));$('chessResult').textContent=JSON.stringify(d,null,2);toast('Chess.com public player data loaded.','success');}\n"
"    catch(e){$('chessResult').textContent='Lookup failed: '+e.message;toast('Chess.com lookup failed.','error');}\n"
"  }\n"
"  async function chessImport(){\n"
"    const u=hubURL(),name=$('chessUser').value.trim(),y=$('pgnYear').value,m=String($('pgnMonth').value).padStart(2,'0');if(!name)return;\n"
"    try{const r=await fetch(u+`/api/chesscom/games/${encodeURIComponent(name)}/${y}/${m}`);const t=await r.text();if(!r.ok)throw new Error(t||('HTTP '+r.status));$('pgnBox').value=t;toast('Chess.com public PGN imported.','success');}\n"
"    catch(e){$('pgnBox').value='Import failed: '+e.message;toast('PGN import failed.','error');}\n"
"  }\n"
"\n"
"  function updateTelemetry(d){\n"
"    const physical={ip:d.ip,active:d.active,rawActiveCells:d.rawActiveCells,rawRowMask:d.rawRowMask,rawColMask:d.rawColMask,multipleRawDetected:d.multipleRawDetected,diagnosticMessage:d.diagnosticMessage,matrix:d.matrix};\n"
"    const led={brightness:d.brightness,speed:d.speed,tracking:d.tracking,animation:d.animation,animationColorCount:d.animationColorCount,animationColors:d.animationColors,trackingColor:d.trackingColor,gameP1Color:d.gameP1Color,gameP2Color:d.gameP2Color,gameGuideColor:d.gameGuideColor,gameWarningColor:d.gameWarningColor,gameTargetColor:d.gameTargetColor,gameEnemyColor:d.gameEnemyColor,gameBallColor:d.gameBallColor,gameWallColor:d.gameWallColor,gameTrailColor:d.gameTrailColor,customFrameCount:d.customFrameCount,customFrameDelay:d.customFrameDelay,ledColors:d.ledColors};\n"
"    const map={rowPins:d.rowPins,colPins:d.colPins,rowMap:d.rowMap,colMap:d.colMap,rowText:d.rowText,colText:d.colText,calibrationRunning:d.calibrationRunning,calibratedSquares:d.calibratedSquares,calibrationStepName:d.calibrationStepName,calibrationStatus:d.calibrationStatus,calibrationSummary:d.calibrationSummary};\n"
"    const game={gameActive:d.gameActive,gameMode:d.gameMode,gameStatus:d.gameStatus,gameScore:d.gameScore,gameMoves:d.gameMoves,gameLives:d.gameLives,gameTarget:d.gameTarget,gameWaiting:d.gameWaiting,gameMovingPlayer:d.gameMovingPlayer,gameP1R:d.gameP1R,gameP1C:d.gameP1C,gameP2R:d.gameP2R,gameP2C:d.gameP2C,pongP1Score:d.pongP1Score,pongP2Score:d.pongP2Score,tetrisLines:d.tetrisLines,snakeLength:d.snakeLength,whackSeconds:d.whackSeconds,chip:d.chip,heap:d.heap};\n"
"    set('telemetryPhysical',JSON.stringify(physical,null,2));set('telemetryLED',JSON.stringify(led,null,2));set('telemetryMap',JSON.stringify(map,null,2));set('telemetryGame',JSON.stringify(game,null,2));set('rawTelemetry',JSON.stringify(d,null,2));\n"
"    renderTelemetryMap('telemetrySensorMap', d.matrix, null);\n"
"    renderTelemetryMap('telemetryLEDMap', null, d.ledColors);\n"
"  }\n"
"\n"
"  function renderTelemetryMap(id,matrix,colors){\n"
"    const box=$(id); if(!box)return; box.innerHTML='';\n"
"    for(let r=7;r>=0;r--) for(let c=0;c<8;c++){\n"
"      const i=r*8+c, cell=document.createElement('div'); cell.className='telemetry-cell';\n"
"      if(matrix?.[r]?.[c]) cell.classList.add('active');\n"
"      if(colors?.[i] && colors[i] !== '#05090C' && colors[i] !== '#000000'){cell.style.background=colors[i];cell.style.boxShadow=`0 0 12px ${colors[i]}, inset 0 0 5px rgba(255,255,255,.12)`;}\n"
"      const sp=document.createElement('span'); sp.textContent=String.fromCharCode(65+c)+(r+1); cell.appendChild(sp); box.appendChild(cell);\n"
"    }\n"
"  }\n"
"\n"
"\n"
"  const NAV_AR={home:'لوحة القيادة',play:'لعب الشطرنج',led:'مختبر الإضاءة',learn:'تعلم',puzzles:'ألغاز',games:'سجل الألعاب',analysis:'التحليل',connect:'الاتصالات',profile:'الملف الشخصي',settings:'الإعدادات',telemetry:'التشخيص المباشر'};\n"
"  function applyTheme(){const t=localStorage.getItem('smartchess.theme')||'prime';document.body.classList.remove('theme-midnight','theme-frost','theme-prime');document.body.classList.add('theme-'+t);if($('theme'))$('theme').value=t;}\n"
"  function applyLanguage(){const lang=localStorage.getItem('smartchess.language')||'English';document.documentElement.lang=lang==='Arabic'?'ar':'en';document.body.classList.toggle('lang-ar',lang==='Arabic');document.querySelectorAll('.nav-item').forEach(b=>{const key=b.dataset.page;const span=b.querySelector('span:last-child');if(span)span.textContent=lang==='Arabic'?(NAV_AR[key]||span.textContent):(pageNames[key]||span.textContent);});renderTop();}\n"
"\n"
"  function bindMobile(){\n"
"    $('mobileMenu')?.addEventListener('click',()=>$('sidebar')?.classList.toggle('open'));\n"
"    document.querySelectorAll('.nav-item').forEach(b=>b.addEventListener('click',()=>{$('sidebar')?.classList.remove('open');setTimeout(()=>{if(b.dataset.page==='led')loadPalette();if(b.dataset.page==='telemetry')poll();if(b.dataset.page==='connect'){hubHealth();chessStatus();aiStatus();}},80);}));\n"
"  }\n"
"\n"
"  function bind(){\n"
"    applyTheme();applyLanguage();\n"
"    A.board=localStorage.getItem('sc.boardUrl')||localStorage.getItem('smartchess.boardUrl')||$('boardUrl')?.value||location.origin;\n"
"    if($('boardUrl'))$('boardUrl').value=A.board;\n"
"    if($('aiUrl'))$('aiUrl').value=localStorage.getItem('sc.aiUrl')||'http://localhost:8080';\n"
"    bindMobile();\n"
"    $('refreshBtn')?.addEventListener('click',poll);\n"
"    $('heroPlay')?.addEventListener('click',()=>document.querySelector('[data-page=\"play\"]')?.click());\n"
"    $('heroLed')?.addEventListener('click',()=>document.querySelector('[data-page=\"led\"]')?.click());\n"
"    $('heroTelemetry')?.addEventListener('click',()=>document.querySelector('[data-page=\"telemetry\"]')?.click());\n"
"    $('startChess')?.addEventListener('click',()=>document.getElementById('startGame')?.click());\n"
"    $('resetChess')?.addEventListener('click',()=>document.getElementById('resetGame')?.click());$('replayStart')?.addEventListener('click',()=>replayStep(-99999));$('replayBack')?.addEventListener('click',()=>replayStep(-1));$('replayForward')?.addEventListener('click',()=>replayStep(1));$('replayEnd')?.addEventListener('click',()=>replayStep(99999));\n"
"    $('hintChess')?.addEventListener('click',()=>document.getElementById('sendHint')?.click());\n"
"    $('qOff')?.addEventListener('click',()=>sendCmd('/alloff'));\n"
"    $('qOn')?.addEventListener('click',()=>sendCmd('/allon?color=%23FFFFFF'));\n"
"    $('qMatrix')?.addEventListener('click',()=>sendCmd('/matrix'));\n"
"    $('qCenter')?.addEventListener('click',()=>selectAnim('centerout'));\n"
"    $('qRainbow')?.addEventListener('click',()=>selectAnim('rainbow'));\n"
"    $('qCustom')?.addEventListener('click',async()=>{await loadCustom();document.querySelector('[data-page=\"led\"]')?.click();selectAnim('custom');});\n"
"    $('offChess')?.addEventListener('click',()=>sendCmd('/alloff'));\n"
"    $('settingsClearLeds')?.addEventListener('click',()=>sendCmd('/alloff'));\n"
"    $('ledOff')?.addEventListener('click',()=>selectAnim('none'));\n"
"    $('ledOn')?.addEventListener('click',()=>sendCmd('/allon?color=%23FFFFFF'));\n"
"    $('ledMatrix')?.addEventListener('click',()=>sendCmd('/matrix'));\n"
"    $('brightRange')?.addEventListener('input',e=>{set('brightLabel',e.target.value);});$('brightRange')?.addEventListener('change',e=>sendCmd('/brightness?value='+e.target.value));\n"
"    $('speedRange')?.addEventListener('input',e=>{set('speedLabel',e.target.value);});$('speedRange')?.addEventListener('change',e=>sendCmd('/speed?value='+e.target.value));\n"
"    document.querySelectorAll('.anim-btn').forEach(b=>b.addEventListener('click',()=>{const k=b.dataset.anim;if(k==='custom')loadCustom().then(()=>selectAnim('custom'));else selectAnim(k);}));\n"
"    $('savePalette')?.addEventListener('click',savePalette);$('savePieceColors')?.addEventListener('click',savePieces);\n"
"    $('customFrames')?.addEventListener('change',()=>{ensureCustom().frames=Number($('customFrames').value);while(ensureCustom().framesData.length<ensureCustom().frames)ensureCustom().framesData.push(Array(64).fill(0));renderCustom();});\n"
"    $('customColors')?.addEventListener('change',()=>{ensureCustom().colors=Number($('customColors').value);renderCustom();});\n"
"    $('customDelay')?.addEventListener('input',()=>ensureCustom().delay=Number($('customDelay').value));$('customRepeat')?.addEventListener('input',()=>ensureCustom().repeatSeconds=Math.max(0,Number($('customRepeat').value)||0));$('customName')?.addEventListener('input',()=>ensureCustom().name=$('customName').value.slice(0,18));\n"
"    $('loadCustom')?.addEventListener('click',loadCustom);$('customSlot')?.addEventListener('change',loadCustomSlot);$('clearFrame')?.addEventListener('click',clearFrame);$('repeatFrame')?.addEventListener('click',repeatCurrentFrame);$('saveCustom')?.addEventListener('click',saveCustomAnimation);$('deleteCustom')?.addEventListener('click',deleteCustomAnimation);\n"
"    $('saveBoard')?.addEventListener('click',saveBoard);$('testBoard')?.addEventListener('click',testBoard);$('hardSave')?.addEventListener('click',hardSave);\n"
"    $('saveAi')?.addEventListener('click',()=>{localStorage.setItem('sc.aiUrl',($('aiUrl').value.trim()||'http://localhost:8080'));localStorage.setItem('sc.aiDepth',$('aiDepth')?.value||16);aiStatus();});\n"
"    $('aiStatus')?.addEventListener('click',aiStatus);$('aiStatusBtn')?.addEventListener('click',aiStatus);$('aiAnalyzeBtn')?.addEventListener('click',aiAnalyze);\n"
"    $('chessStatus')?.addEventListener('click',chessStatus);$('chessLookup')?.addEventListener('click',chessLookup);$('chessImport')?.addEventListener('click',chessImport);\n"
"    $('connectOnline')?.addEventListener('click',()=>{localStorage.setItem('sc.hubUrl',hubURL());const legacy=$('connectOnline');void legacy; window.dispatchEvent(new CustomEvent('smartchess:online-connect'));});\n"
"    $('saveSettings')?.addEventListener('click',saveSettings);$('theme')?.addEventListener('change',()=>{localStorage.setItem('smartchess.theme',$('theme').value);applyTheme();});$('language')?.addEventListener('change',()=>{S.settings.language=$('language').value;localStorage.setItem('smartchess.language',S.settings.language);applyLanguage();});\n"
"    $('copyTelemetry')?.addEventListener('click',async()=>{try{await navigator.clipboard.writeText($('rawTelemetry').textContent||'');toast('Telemetry JSON copied.','success');}catch(e){toast('Clipboard unavailable.','error');}});\n"
"    if($('hubUrl'))$('hubUrl').value=localStorage.getItem('sc.hubUrl')||'';\n"
"    $('hubTest')?.addEventListener('click',hubHealth);\n"
"    $('hubUrl')?.addEventListener('change',()=>localStorage.setItem('sc.hubUrl',$('hubUrl').value.trim()));\n"
"    setInterval(poll,1000);poll();loadCustom();\n"
"  }\n"
"\n"
"  window.addEventListener('load',bind);\n"
"})();\n"
"\n"
"</script>\n"
"</body>\n"
"</html>\n";

// ============================================================
// SMART CHESS  -  8x8 SENSITIVE TRACKING FIRMWARE
// ESP32-WROOM-32U
// 64 REED SWITCHES + 64 ADDRESSABLE RGB LEDs
//
// IMPORTANT:
// - Row/column ORDER can be changed from the web page.
// - Calibration can automatically discover the physical row/column
//   for each logical row/column using one magnet at a time.
// - The diagnostic page shows RAW physical row/column activity.
// - If ONE magnet produces MANY physical columns/rows, that is not
//   a coordinate-order problem; it points to wiring/ghosting/shorts.
// ============================================================

#include <stdint.h>
#include <string.h>

// -----------------------------
// Wi-Fi Access Point
// -----------------------------
const char* AP_SSID = "SmartChess-Test";
const char* AP_PASSWORD = "12345678";

WebServer server(80);
Preferences prefs;

// Forward declarations used by settings/game color code.
void loadGameColors();
void saveGameColors();
void gameGetTwoPiecePosition(uint8_t player, uint8_t &r, uint8_t &c);

// -----------------------------
// LED configuration
// -----------------------------
#define LED_PIN       2
#define NUM_LEDS      64
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB

CRGB leds[NUM_LEDS];
uint8_t brightness = 80;

// -----------------------------
// Reed matrix configuration
// -----------------------------
const uint8_t ROWS = 8;
const uint8_t COLS = 8;

// These are the physical GPIOs actually wired to the row lines.
// DO NOT change these unless your wiring uses different GPIOs.


// Logical -> physical line mapping.
// Example: rowMap[0] = 7 means logical Row 1 uses physical row GPIO 22.
uint8_t rowMap[ROWS];
uint8_t colMap[COLS];

// Actual physical GPIO assignments are editable from the hardware console.
uint8_t defaultRowPins[ROWS] = {13,14,16,17,18,19,21,22};
uint8_t defaultColPins[COLS] = {23,25,26,27,32,33,4,5};

// Current stable logical matrix state.
bool matrixState[ROWS][COLS];
bool previousMatrixState[ROWS][COLS];

// Raw physical masks from the last scan.
uint8_t rawRowMask = 0;
uint8_t rawColMask = 0;
uint8_t rawActiveCells = 0;

// Diagnostic information.
bool diagnosticMode = true;
bool multipleRawDetected = false;
String diagnosticMessage = "Ready";

// Game hooks are defined later in the file.
extern bool gameActive;
void gameProcessBoardChange();

// Last changed square.
String lastEvent = "Waiting for switch activity";
String lastSquare = "--";
unsigned long lastEventTime = 0;

// -----------------------------
// Sensitivity / response control
// -----------------------------
bool candidateMatrix[ROWS][COLS];
uint8_t stableCounter[ROWS][COLS];

// 10 = fastest / most sensitive software response.
// 1  = strongest filtering / slowest response.
uint8_t responseSensitivity = 10;

// How long each physical row is allowed to settle before reading columns.
// Lower = faster. Higher = more filtering.
uint16_t scanSettleUs = 8;

bool matrixChangedThisScan = false;
unsigned long lastMatrixScan = 0;
const uint16_t MATRIX_SCAN_INTERVAL_MS = 1;

// -----------------------------
// LED animation configuration
// -----------------------------
enum AnimationMode {
  ANIM_NONE,
  ANIM_RAINBOW,
  ANIM_WAVE,
  ANIM_CHASE,
  ANIM_SCAN,
  ANIM_BREATH,
  ANIM_RANDOM,
  ANIM_CENTER_OUT,
  ANIM_CUSTOM
};

#define BUILTIN_ANIMATION_COUNT 7
#define MAX_ANIMATION_COLORS 8
#define CUSTOM_MAX_FRAMES 8
#define CUSTOM_MAX_COLORS 8
#define CUSTOM_SLOT_COUNT 8

AnimationMode animationMode = ANIM_NONE;
uint8_t animationSpeed = 100;
uint8_t animationHue = 0;
unsigned long lastAnimationUpdate = 0;

bool manualLEDMode = true;

CRGB selectedColor = CRGB(255, 255, 255);
CRGB animationColor1 = CRGB(0, 120, 255);
CRGB animationColor2 = CRGB(255, 0, 100);

// Each built-in animation has its own independent color palette.
// Counts: Rainbow 6, Wave 2, Chase 3, Scan 3, Breath 1, Random 4, Center Out 2.
const uint8_t animationPaletteCounts[BUILTIN_ANIMATION_COUNT] = {6,2,3,3,1,4,2};
CRGB animationPalettes[BUILTIN_ANIMATION_COUNT][MAX_ANIMATION_COLORS];

// User-created animation: up to 8 frames, each frame is an 8x8 paint grid.
// 0 = OFF, 1..8 = palette color index.
CRGB customAnimationColors[CUSTOM_MAX_COLORS];
uint8_t customAnimationColorCount = 4;
uint8_t customAnimationFrames[CUSTOM_MAX_FRAMES][64];
uint8_t customAnimationFrameCount = 1;
uint16_t customAnimationFrameDelay = 120;
uint8_t customAnimationFrame = 0;
uint16_t customAnimationRepeatSeconds = 0;
unsigned long customAnimationStartedAt = 0;
uint8_t customActiveSlot = 0;
struct CustomSlotMeta { bool used; uint8_t colorCount; uint8_t frameCount; uint16_t frameDelay; uint16_t repeatSeconds; char name[19]; };
CustomSlotMeta customSlots[CUSTOM_SLOT_COUNT];

// Animation colors and other control values can be HARD-SAVED
// into ESP32 non-volatile flash using Preferences.
uint32_t packRGB(CRGB color) {
  return ((uint32_t)color.r << 16) | ((uint32_t)color.g << 8) | color.b;
}

CRGB unpackRGB(uint32_t value) {
  return CRGB(
    (value >> 16) & 0xFF,
    (value >> 8) & 0xFF,
    value & 0xFF
  );
}

uint8_t animationPaletteIndex(AnimationMode mode) {
  switch (mode) {
    case ANIM_RAINBOW: return 0;
    case ANIM_WAVE: return 1;
    case ANIM_CHASE: return 2;
    case ANIM_SCAN: return 3;
    case ANIM_BREATH: return 4;
    case ANIM_RANDOM: return 5;
    case ANIM_CENTER_OUT: return 6;
    default: return 0;
  }
}

void setDefaultAnimationPalettes() {
  for (uint8_t a = 0; a < BUILTIN_ANIMATION_COUNT; a++) {
    for (uint8_t c = 0; c < MAX_ANIMATION_COLORS; c++) animationPalettes[a][c] = CRGB::Black;
  }

  // Rainbow - six fully editable colors.
  animationPalettes[0][0] = CRGB(255, 0, 0);
  animationPalettes[0][1] = CRGB(255, 110, 0);
  animationPalettes[0][2] = CRGB(255, 240, 0);
  animationPalettes[0][3] = CRGB(0, 220, 90);
  animationPalettes[0][4] = CRGB(0, 140, 255);
  animationPalettes[0][5] = CRGB(170, 60, 255);

  // Wave - two-color gradient.
  animationPalettes[1][0] = CRGB(0, 120, 255);
  animationPalettes[1][1] = CRGB(255, 0, 120);

  // Chase - three-color moving trail.
  animationPalettes[2][0] = CRGB(0, 0, 0);
  animationPalettes[2][1] = CRGB(0, 210, 255);
  animationPalettes[2][2] = CRGB(255, 60, 180);

  // Scan - three-color head / trail / background.
  animationPalettes[3][0] = CRGB(0, 0, 0);
  animationPalettes[3][1] = CRGB(0, 220, 255);
  animationPalettes[3][2] = CRGB(80, 90, 255);

  // Breath - one editable color.
  animationPalettes[4][0] = CRGB(0, 170, 255);

  // Random - four editable colors.
  animationPalettes[5][0] = CRGB(255, 40, 40);
  animationPalettes[5][1] = CRGB(255, 170, 20);
  animationPalettes[5][2] = CRGB(40, 230, 120);
  animationPalettes[5][3] = CRGB(90, 100, 255);

  // Center Out - two-color rings + center highlight.
  animationPalettes[6][0] = CRGB(0, 120, 255);
  animationPalettes[6][1] = CRGB(255, 0, 120);

  animationColor1 = animationPalettes[6][0];
  animationColor2 = animationPalettes[6][1];
}

void setDefaultCustomAnimation() {
  customAnimationColorCount = 4;
  customAnimationColors[0] = CRGB(0, 210, 255);
  customAnimationColors[1] = CRGB(255, 60, 180);
  customAnimationColors[2] = CRGB(255, 210, 40);
  customAnimationColors[3] = CRGB(80, 255, 120);
  for (uint8_t c = 4; c < CUSTOM_MAX_COLORS; c++) customAnimationColors[c] = CRGB::Black;
  customAnimationFrameCount = 1;
  customAnimationFrameDelay = 120;
  customAnimationRepeatSeconds = 0;
  customAnimationStartedAt = 0;
  customAnimationFrame = 0;
  memset(customAnimationFrames, 0, sizeof(customAnimationFrames));
  // A simple default star/cross frame so CUSTOM is visibly usable immediately.
  const uint8_t pts[][2] = {{3,3},{3,4},{4,3},{4,4},{2,3},{3,2},{3,5},{4,2},{4,5},{5,3},{5,4}};
  for (uint8_t i = 0; i < sizeof(pts)/sizeof(pts[0]); i++) {
    uint8_t r = pts[i][0], c = pts[i][1];
    customAnimationFrames[0][r * 8 + c] = (i % customAnimationColorCount) + 1;
  }
}

void syncCurrentAnimationColors() {
  if (animationMode >= ANIM_RAINBOW && animationMode <= ANIM_CENTER_OUT) {
    uint8_t idx = animationPaletteIndex(animationMode);
    animationColor1 = animationPalettes[idx][0];
    animationColor2 = animationPalettes[idx][1];
  }
}

void saveAnimationPalettes() {
  prefs.begin("smartchess", false);
  prefs.putBytes("animpal", animationPalettes, sizeof(animationPalettes));
  prefs.putBytes("customclr", customAnimationColors, sizeof(customAnimationColors));
  prefs.putUChar("customcc", customAnimationColorCount);
  prefs.putBytes("customfr", customAnimationFrames, sizeof(customAnimationFrames));
  prefs.putUChar("customfc", customAnimationFrameCount);
  prefs.putUShort("customfd", customAnimationFrameDelay);
  prefs.putUShort("customrs", customAnimationRepeatSeconds);
  prefs.putUChar("customslot", customActiveSlot);
  prefs.end();
}

void loadAnimationPalettes() {
  setDefaultAnimationPalettes();
  setDefaultCustomAnimation();
  prefs.begin("smartchess", true);
  size_t plen = prefs.getBytesLength("animpal");
  if (plen == sizeof(animationPalettes)) prefs.getBytes("animpal", animationPalettes, sizeof(animationPalettes));
  size_t clen = prefs.getBytesLength("customclr");
  if (clen == sizeof(customAnimationColors)) prefs.getBytes("customclr", customAnimationColors, sizeof(customAnimationColors));
  customAnimationColorCount = prefs.getUChar("customcc", customAnimationColorCount);
  if (customAnimationColorCount < 1 || customAnimationColorCount > CUSTOM_MAX_COLORS) customAnimationColorCount = 4;
  size_t flen = prefs.getBytesLength("customfr");
  if (flen == sizeof(customAnimationFrames)) prefs.getBytes("customfr", customAnimationFrames, sizeof(customAnimationFrames));
  customAnimationFrameCount = prefs.getUChar("customfc", customAnimationFrameCount);
  if (customAnimationFrameCount < 1 || customAnimationFrameCount > CUSTOM_MAX_FRAMES) customAnimationFrameCount = 1;
  customAnimationFrameDelay = prefs.getUShort("customfd", customAnimationFrameDelay);
  customAnimationRepeatSeconds = prefs.getUShort("customrs", 0);
  if(customAnimationRepeatSeconds>600) customAnimationRepeatSeconds=0;
  customActiveSlot = prefs.getUChar("customslot",0);
  if(customActiveSlot>=CUSTOM_SLOT_COUNT) customActiveSlot=0;
  if (customAnimationFrameDelay < 20 || customAnimationFrameDelay > 2000) customAnimationFrameDelay = 120;
  prefs.end();
  syncCurrentAnimationColors();
}


void saveCustomSlotToNVS(uint8_t slot, const char* name) {
  if(slot>=CUSTOM_SLOT_COUNT)return;
  CustomSlotMeta meta{};meta.used=true;meta.colorCount=customAnimationColorCount;meta.frameCount=customAnimationFrameCount;meta.frameDelay=customAnimationFrameDelay;meta.repeatSeconds=customAnimationRepeatSeconds;
  strncpy(meta.name,name?name:"My Animation",sizeof(meta.name)-1);
  prefs.begin("smartchess", false);
  char key[12];snprintf(key,sizeof(key),"s%umeta",slot);prefs.putBytes(key,&meta,sizeof(meta));
  snprintf(key,sizeof(key),"s%uclr",slot);prefs.putBytes(key,customAnimationColors,sizeof(customAnimationColors));
  snprintf(key,sizeof(key),"s%ufr",slot);prefs.putBytes(key,customAnimationFrames,sizeof(customAnimationFrames));
  prefs.end();
}

bool loadCustomSlotFromNVS(uint8_t slot) {
  if(slot>=CUSTOM_SLOT_COUNT)return false;CustomSlotMeta meta{};
  prefs.begin("smartchess", true);char key[12];snprintf(key,sizeof(key),"s%umeta",slot);bool ok=prefs.getBytesLength(key)==sizeof(meta);if(ok)prefs.getBytes(key,&meta,sizeof(meta));
  if(!ok||!meta.used){prefs.end();return false;}
  if(meta.colorCount<1||meta.colorCount>CUSTOM_MAX_COLORS||meta.frameCount<1||meta.frameCount>CUSTOM_MAX_FRAMES){prefs.end();return false;}
  snprintf(key,sizeof(key),"s%uclr",slot);if(prefs.getBytesLength(key)==sizeof(customAnimationColors))prefs.getBytes(key,customAnimationColors,sizeof(customAnimationColors));
  snprintf(key,sizeof(key),"s%ufr",slot);if(prefs.getBytesLength(key)==sizeof(customAnimationFrames))prefs.getBytes(key,customAnimationFrames,sizeof(customAnimationFrames));
  customAnimationColorCount=meta.colorCount;customAnimationFrameCount=meta.frameCount;customAnimationFrameDelay=meta.frameDelay;customAnimationRepeatSeconds=meta.repeatSeconds;customAnimationFrame=0;customActiveSlot=slot;
  prefs.end();saveAnimationPalettes();return true;
}

void deleteCustomSlotNVS(uint8_t slot) {
  if(slot>=CUSTOM_SLOT_COUNT)return;prefs.begin("smartchess",false);char key[12];snprintf(key,sizeof(key),"s%umeta",slot);CustomSlotMeta meta{};meta.used=false;snprintf(key,sizeof(key),"s%umeta",slot);prefs.putBytes(key,&meta,sizeof(meta));prefs.end();
}

String customSlotName(uint8_t slot) {
  if(slot>=CUSTOM_SLOT_COUNT)return "";CustomSlotMeta meta{};prefs.begin("smartchess",true);char key[12];snprintf(key,sizeof(key),"s%umeta",slot);String n="";if(prefs.getBytesLength(key)==sizeof(meta)){prefs.getBytes(key,&meta,sizeof(meta));if(meta.used)n=String(meta.name);}prefs.end();return n;
}

bool parseHexColor(const String &value, CRGB &out) {
  if (value.length() != 7 || value.charAt(0) != '#') return false;

  char *endPtr = nullptr;
  unsigned long number = strtoul(value.substring(1).c_str(), &endPtr, 16);
  if (endPtr == nullptr || *endPtr != '\0' || number > 0xFFFFFFUL) return false;

  out = CRGB(
    (number >> 16) & 0xFF,
    (number >> 8) & 0xFF,
    number & 0xFF
  );
  return true;
}

// Automatic piece-position LED tracking.
// When a reed switch becomes active, its matching LED turns on.
// When the magnet moves, the old LED turns off and the new LED turns on.
bool pieceTrackingEnabled = true;
CRGB pieceTrackingColor = CRGB(0, 255, 80);

// -----------------------------
// Full 64-square calibration state
// -----------------------------
// Every logical square is calibrated one-by-one.
// The physical row/column pair detected for each square is saved in flash
// after every successful capture, so progress is not lost if the ESP32 restarts.
uint8_t calibrationStep = 0;       // 0..63
bool calibrationRunning = false;
String calibrationStatus = "Calibration is not running.";
String calibrationSummary = "No completed calibration yet.";

bool calibrationCaptured[64];
uint8_t calibrationPhysicalRow[64];
uint8_t calibrationPhysicalCol[64];

// ============================================================
// Utility
// ============================================================

String squareName(uint8_t row, uint8_t col) {
  String s = "";
  s += char('A' + col);
  s += String(row + 1);
  return s;
}

String jsonEscape(String value) {
  value.replace("\\", "\\\\");
  value.replace("\"", "\\\"");
  value.replace("\n", "\\n");
  value.replace("\r", "\\r");
  return value;
}

bool isValidPhysicalRow(uint8_t p) {
  return p < ROWS;
}

bool isValidPhysicalCol(uint8_t p) {
  return p < COLS;
}

uint8_t logicalRowPin(uint8_t logicalRow) {
  return defaultRowPins[rowMap[logicalRow]];
}

uint8_t logicalColPin(uint8_t logicalCol) {
  return defaultColPins[colMap[logicalCol]];
}

String mappingToText(const uint8_t mapArray[8], const uint8_t pinArray[8]) {
  String s = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (i) s += ", ";
    s += String(i + 1);
    s += "->GPIO";
    s += String(pinArray[mapArray[i]]);
  }
  return s;
}

// ============================================================
// Persistent mapping
// ============================================================

void setDefaultMapping() {
  // Last confirmed 64-square calibration from this board.
  // Rows: 1,2,3,4,5,6,7,8 -> physical 1..8.
  // Columns: 1..8 -> physical 4,7,8,1,5,3,6,2.
  const uint8_t calibratedRows[8] = {0,1,2,3,4,5,6,7};
  const uint8_t calibratedCols[8] = {3,6,7,0,4,2,5,1};
  for (uint8_t i = 0; i < 8; i++) {
    rowMap[i] = calibratedRows[i];
    colMap[i] = calibratedCols[i];
  }
}

void loadMapping() {
  setDefaultMapping();

  prefs.begin("smartchess", true);

  for (uint8_t i = 0; i < 8; i++) {
    char rKey[3] = {'r', char('0' + i), '\0'};
    char cKey[3] = {'c', char('0' + i), '\0'};
    uint8_t r = prefs.getUChar(rKey, i);
    uint8_t c = prefs.getUChar(cKey, i);
    rowMap[i] = isValidPhysicalRow(r) ? r : i;
    colMap[i] = isValidPhysicalCol(c) ? c : i;
  }

  prefs.end();
}

bool mappingIsUnique(const uint8_t mapArray[8]) {
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = i + 1; j < 8; j++) {
      if (mapArray[i] == mapArray[j]) return false;
    }
  }
  return true;
}

void saveMapping() {
  prefs.begin("smartchess", false);

  for (uint8_t i = 0; i < 8; i++) {
    char rKey[3] = {'r', char('0' + i), '\0'};
    char cKey[3] = {'c', char('0' + i), '\0'};
    prefs.putUChar(rKey, rowMap[i]);
    prefs.putUChar(cKey, colMap[i]);
  }

  prefs.end();
}

void resetMapping() {
  setDefaultMapping();
  saveMapping();
}

bool pinAllowedForMatrix(uint8_t p, bool outputPin) {
  if (p > 39) return false;
  // Avoid flash pins and the USB/UART pins used by Serial.
  if (p >= 6 && p <= 11) return false;
  if (p == 1 || p == 3) return false;
  if (outputPin && p >= 34) return false;
  if (p == LED_PIN) return false;
  return true;
}

bool pinsUniqueAll(const uint8_t *a, const uint8_t *b) {
  for (uint8_t i=0;i<8;i++) for (uint8_t j=0;j<8;j++) if (a[i] == b[j]) return false;
  return true;
}

void savePinConfig() {
  prefs.begin("smartchess", false);
  prefs.putBytes("rowpins", defaultRowPins, sizeof(defaultRowPins));
  prefs.putBytes("colpins", defaultColPins, sizeof(defaultColPins));
  prefs.end();
}

void loadPinConfig() {
  const uint8_t safeRows[8] = {13,14,16,17,18,19,21,22};
  const uint8_t safeCols[8] = {23,25,26,27,32,33,4,5};
  memcpy(defaultRowPins, safeRows, sizeof(defaultRowPins));
  memcpy(defaultColPins, safeCols, sizeof(defaultColPins));
  prefs.begin("smartchess", true);
  if (prefs.getBytesLength("rowpins") == sizeof(defaultRowPins)) prefs.getBytes("rowpins", defaultRowPins, sizeof(defaultRowPins));
  if (prefs.getBytesLength("colpins") == sizeof(defaultColPins)) prefs.getBytes("colpins", defaultColPins, sizeof(defaultColPins));
  prefs.end();
  bool ok=true;
  for(uint8_t i=0;i<8;i++){if(!pinAllowedForMatrix(defaultRowPins[i],true))ok=false;if(!pinAllowedForMatrix(defaultColPins[i],false))ok=false;}
  if(!pinsUniqueAll(defaultRowPins,defaultColPins)){ok=false;}
  if(!ok){memcpy(defaultRowPins,safeRows,sizeof(defaultRowPins));memcpy(defaultColPins,safeCols,sizeof(defaultColPins));savePinConfig();}
}

void setupMatrixPins() {
  for(uint8_t r=0;r<ROWS;r++){pinMode(defaultRowPins[r],OUTPUT);digitalWrite(defaultRowPins[r],LOW);}
  for(uint8_t c=0;c<COLS;c++)pinMode(defaultColPins[c],INPUT_PULLDOWN);
}

// ============================================================
// Sensitivity settings
// ============================================================

uint8_t requiredStableScans() {
  return (uint8_t)(11 - constrain((int)responseSensitivity, 1, 10));
}

void loadAllSettings() {
  prefs.begin("smartchess", true);

  responseSensitivity = prefs.getUChar("sens", 10);
  if (responseSensitivity < 1 || responseSensitivity > 10) {
    responseSensitivity = 10;
  }

  scanSettleUs = prefs.getUShort("settle", 8);
  if (scanSettleUs < 2 || scanSettleUs > 100) {
    scanSettleUs = 8;
  }

  pieceTrackingEnabled = prefs.getBool("track", true);

  brightness = prefs.getUChar("bright", 80);
  if (brightness < 1) brightness = 80;

  animationSpeed = prefs.getUChar("speed", 100);
  if (animationSpeed < 10) animationSpeed = 100;

  animationColor1 = unpackRGB(
    prefs.getUInt("anim1", packRGB(CRGB(0, 120, 255)))
  );

  animationColor2 = unpackRGB(
    prefs.getUInt("anim2", packRGB(CRGB(255, 0, 100)))
  );

  pieceTrackingColor = unpackRGB(
    prefs.getUInt("trackclr", packRGB(CRGB(0, 255, 80)))
  );

  prefs.end();
  loadAnimationPalettes();
}

void saveAllSettings() {
  prefs.begin("smartchess", false);
  prefs.putUChar("sens", responseSensitivity);
  prefs.putUShort("settle", scanSettleUs);
  prefs.putBool("track", pieceTrackingEnabled);
  prefs.putUChar("bright", brightness);
  prefs.putUChar("speed", animationSpeed);
  prefs.putUInt("anim1", packRGB(animationColor1));
  prefs.putUInt("anim2", packRGB(animationColor2));
  prefs.putUInt("trackclr", packRGB(pieceTrackingColor));
  prefs.end();
}

// ============================================================
// Persistent 64-square calibration progress
// ============================================================

void clearCalibrationProgressStorage() {
  prefs.begin("smartchess", false);
  prefs.putUChar("calActive", 0);
  prefs.putUChar("calStep", 0);
  for (uint8_t i = 0; i < 64; i++) {
    char key[5];
    snprintf(key, sizeof(key), "cp%02u", i);
    prefs.putUChar(key, 0);
  }
  prefs.end();
}

void loadCalibrationProgress() {
  memset(calibrationCaptured, 0, sizeof(calibrationCaptured));
  memset(calibrationPhysicalRow, 0, sizeof(calibrationPhysicalRow));
  memset(calibrationPhysicalCol, 0, sizeof(calibrationPhysicalCol));

  prefs.begin("smartchess", true);
  uint8_t active = prefs.getUChar("calActive", 0);
  calibrationStep = prefs.getUChar("calStep", 0);
  prefs.end();

  if (calibrationStep > 63) calibrationStep = 63;

  for (uint8_t i = 0; i < 64; i++) {
    char key[5];
    snprintf(key, sizeof(key), "cp%02u", i);

    prefs.begin("smartchess", true);
    uint8_t packed = prefs.getUChar(key, 0);
    prefs.end();

    if (packed & 0x80) {
      calibrationCaptured[i] = true;
      calibrationPhysicalRow[i] = (packed >> 3) & 0x07;
      calibrationPhysicalCol[i] = packed & 0x07;
    }
  }

  calibrationRunning = active != 0;
  if (calibrationRunning) {
    calibrationStatus = "Saved calibration progress loaded. Continue from " + squareName(calibrationStep / 8, calibrationStep % 8) + ".";
  } else {
    calibrationStatus = "Calibration is not running.";
  }
}

void saveCalibrationProgress(bool keepRunning) {
  prefs.begin("smartchess", false);
  prefs.putUChar("calActive", keepRunning ? 1 : 0);
  prefs.putUChar("calStep", calibrationStep);

  for (uint8_t i = 0; i < 64; i++) {
    char key[5];
    snprintf(key, sizeof(key), "cp%02u", i);

    uint8_t packed = 0;
    if (calibrationCaptured[i]) {
      packed = 0x80 | (uint8_t)((calibrationPhysicalRow[i] & 0x07) << 3) | (calibrationPhysicalCol[i] & 0x07);
    }
    prefs.putUChar(key, packed);
  }

  prefs.end();
}

bool deriveFinalMapping() {
  uint8_t derivedRows[8];
  uint8_t derivedCols[8];

  for (uint8_t i = 0; i < 8; i++) {
    derivedRows[i] = 255;
    derivedCols[i] = 255;
  }

  // Every logical row must consistently map to one physical row.
  // Every logical column must consistently map to one physical column.
  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t idx = r * 8 + c;
      if (!calibrationCaptured[idx]) return false;

      uint8_t pr = calibrationPhysicalRow[idx];
      uint8_t pc = calibrationPhysicalCol[idx];

      if (derivedRows[r] == 255) derivedRows[r] = pr;
      else if (derivedRows[r] != pr) return false;

      if (derivedCols[c] == 255) derivedCols[c] = pc;
      else if (derivedCols[c] != pc) return false;
    }
  }

  for (uint8_t i = 0; i < 8; i++) {
    if (derivedRows[i] > 7 || derivedCols[i] > 7) return false;
  }

  if (!mappingIsUnique(derivedRows) || !mappingIsUnique(derivedCols)) return false;

  memcpy(rowMap, derivedRows, sizeof(rowMap));
  memcpy(colMap, derivedCols, sizeof(colMap));

  return true;
}

void loadSensitivitySettings() {
  loadAllSettings();
}

void saveSensitivitySettings() {
  saveAllSettings();
}


// ============================================================
// LED INDEX MAPPING
// ============================================================

// The board is shown Row 1 at the TOP.
// The LED strip in this project starts physically at the BOTTOM-LEFT,
// so logical board rows are flipped when addressing the LED strip.
#define LED_ROWS_REVERSED 1

uint8_t chessToLED(uint8_t row, uint8_t col) {
  uint8_t ledRow = LED_ROWS_REVERSED ? (7 - row) : row;
  if ((ledRow % 2) == 0) {
    return ledRow * 8 + col;
  }
  return ledRow * 8 + (7 - col);
}

void clearLEDs() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void setPhysicalLED(uint8_t index, CRGB color) {
  if (index >= NUM_LEDS) return;
  leds[index] = color;
  FastLED.show();
}

// ============================================================
// Automatic piece-position LED tracking
// ============================================================

void updatePieceTrackingLEDs() {
  if (!pieceTrackingEnabled) return;

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      if (matrixState[r][c]) {
        leds[chessToLED(r, c)] = pieceTrackingColor;
      }
    }
  }

  FastLED.show();
}

// ============================================================
// Physical matrix scan
// ============================================================
// This scan deliberately ignores rowMap/colMap.
// It tells us what the WIRING itself is doing.
// ============================================================

void scanPhysicalMatrix(bool outputToLogicalState) {
  bool physicalCells[ROWS][COLS];
  memset(physicalCells, 0, sizeof(physicalCells));

  rawRowMask = 0;
  rawColMask = 0;
  rawActiveCells = 0;
  matrixChangedThisScan = false;

  // Read each physical row.
  for (uint8_t pr = 0; pr < ROWS; pr++) {

    // All rows LOW.
    for (uint8_t r = 0; r < ROWS; r++) {
      digitalWrite(defaultRowPins[r], LOW);
    }

    // Activate exactly one physical row.
    digitalWrite(defaultRowPins[pr], HIGH);

    delayMicroseconds(scanSettleUs);

    for (uint8_t pc = 0; pc < COLS; pc++) {
      bool state = digitalRead(defaultColPins[pc]);
      physicalCells[pr][pc] = state;

      if (state) {
        rawRowMask |= (1u << pr);
        rawColMask |= (1u << pc);
      }
    }

    digitalWrite(defaultRowPins[pr], LOW);
  }

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      if (physicalCells[r][c]) rawActiveCells++;
    }
  }

  uint8_t rawRows = 0;
  uint8_t rawCols = 0;

  for (uint8_t i = 0; i < 8; i++) {
    if (rawRowMask & (1u << i)) rawRows++;
    if (rawColMask & (1u << i)) rawCols++;
  }

  multipleRawDetected = (rawActiveCells > 1) || (rawRows > 1) || (rawCols > 1);

  if (rawActiveCells == 0) {
    diagnosticMessage = "No reed switch active";
  } else if (rawActiveCells == 1 && rawRows == 1 && rawCols == 1) {
    diagnosticMessage = "Exactly 1 physical switch active  -  good for calibration";
  } else {
    diagnosticMessage = "Multiple physical activations detected  -  check wiring/ghosting";
  }

  if (!outputToLogicalState) return;

  // Convert physical matrix to logical matrix according to the current mapping.
  bool logicalCells[ROWS][COLS];
  memset(logicalCells, 0, sizeof(logicalCells));

  for (uint8_t lr = 0; lr < ROWS; lr++) {
    uint8_t pr = rowMap[lr];

    for (uint8_t lc = 0; lc < COLS; lc++) {
      uint8_t pc = colMap[lc];
      logicalCells[lr][lc] = physicalCells[pr][pc];
    }
  }

  // Debounce each logical square.
  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {

      uint8_t needed = requiredStableScans();

      if (logicalCells[r][c] != candidateMatrix[r][c]) {
        candidateMatrix[r][c] = logicalCells[r][c];
        stableCounter[r][c] = 1;
      } else if (stableCounter[r][c] < needed) {
        stableCounter[r][c]++;
      }

      if (stableCounter[r][c] >= needed &&
          matrixState[r][c] != candidateMatrix[r][c]) {

        previousMatrixState[r][c] = matrixState[r][c];
        matrixState[r][c] = candidateMatrix[r][c];
        matrixChangedThisScan = true;

        String sq = squareName(r, c);

        if (matrixState[r][c]) {
          lastEvent = "SWITCH ON: " + sq;
        } else {
          lastEvent = "SWITCH OFF: " + sq;
        }

        lastSquare = sq;
        lastEventTime = millis();

        Serial.print(lastEvent);
        Serial.print(" | LOGICAL ROW=");
        Serial.print(r + 1);
        Serial.print(" COL=");
        Serial.print(c + 1);
        Serial.print(" | PHYSICAL GPIO ROW=");
        Serial.print(logicalRowPin(r));
        Serial.print(" COL=");
        Serial.println(logicalColPin(c));
      }
    }
  }

  // Update the LEDs immediately when a square changes.
  // Game mode owns the LEDs while the two-piece game is active.
  if (matrixChangedThisScan) {
    if (gameActive) {
      gameProcessBoardChange();
    } else if (pieceTrackingEnabled && animationMode == ANIM_NONE) {
      updatePieceTrackingLEDs();
    }
  }
}

// ============================================================
// Calibration capture
// ============================================================

bool getSinglePhysicalActivation(uint8_t &physicalRow, uint8_t &physicalCol) {
  // A fresh scan of the physical matrix.
  scanPhysicalMatrix(false);

  if (rawActiveCells != 1) {
    return false;
  }

  int foundRow = -1;
  int foundCol = -1;

  // Find which physical row and column are active from the masks.
  for (uint8_t i = 0; i < 8; i++) {
    if (rawRowMask & (1u << i)) foundRow = i;
    if (rawColMask & (1u << i)) foundCol = i;
  }

  if (foundRow < 0 || foundCol < 0) return false;

  physicalRow = (uint8_t)foundRow;
  physicalCol = (uint8_t)foundCol;
  return true;
}

String buildCalibrationSummary() {
  String out = "FINAL CALIBRATION SUMMARY\n\n";

  out += "Rows (Logical -> Physical -> GPIO):\n";
  for (uint8_t i = 0; i < 8; i++) {
    out += "Row " + String(i + 1) + " -> Physical " + String(rowMap[i] + 1);
    out += " -> GPIO " + String(defaultRowPins[rowMap[i]]) + "\n";
  }

  out += "\nColumns (Logical -> Physical -> GPIO):\n";
  for (uint8_t i = 0; i < 8; i++) {
    out += "Col " + String(i + 1) + " -> Physical " + String(colMap[i] + 1);
    out += " -> GPIO " + String(defaultColPins[colMap[i]]) + "\n";
  }

  out += "\nAll 64 square changes:\n";
  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t idx = r * 8 + c;
      out += squareName(r, c);
      out += " -> Physical R";
      out += String(calibrationPhysicalRow[idx] + 1);
      out += ", C";
      out += String(calibrationPhysicalCol[idx] + 1);
      out += " -> GPIO ";
      out += String(defaultRowPins[calibrationPhysicalRow[idx]]);
      out += "/";
      out += String(defaultColPins[calibrationPhysicalCol[idx]]);
      if (!(r == 7 && c == 7)) out += "\n";
    }
  }

  out += "\n\nHARD SAVE: COMPLETE";
  return out;
}

void calibrationReset() {
  memset(calibrationCaptured, 0, sizeof(calibrationCaptured));
  memset(calibrationPhysicalRow, 0, sizeof(calibrationPhysicalRow));
  memset(calibrationPhysicalCol, 0, sizeof(calibrationPhysicalCol));

  calibrationStep = 0;
  calibrationRunning = true;
  calibrationSummary = "No completed calibration yet.";
  calibrationStatus =
    "Place ONE magnet on A1. Press OK in the prompt, then capture.";

  clearCalibrationProgressStorage();
  saveMapping();
}

void calibrationResume() {
  loadCalibrationProgress();

  if (calibrationStep > 63) calibrationStep = 63;
  calibrationRunning = true;

  if (calibrationCaptured[calibrationStep]) {
    while (calibrationStep < 63 && calibrationCaptured[calibrationStep]) {
      calibrationStep++;
    }
    if (calibrationStep >= 63 && calibrationCaptured[63]) {
      calibrationStep = 63;
      if (deriveFinalMapping()) {
        calibrationSummary = buildCalibrationSummary();
      }
      calibrationRunning = false;
      calibrationStatus = "All 64 squares are already calibrated and saved.";
      return;
    }
  }

  calibrationStatus =
    "Resumed. Place ONE magnet on " +
    squareName(calibrationStep / 8, calibrationStep % 8) +
    " and capture it.";
}

void calibrationCapture() {
  if (!calibrationRunning) {
    calibrationStatus =
      "Calibration is not running. Press START 64-SQUARE CALIBRATION.";
    return;
  }

  uint8_t pr = 0;
  uint8_t pc = 0;

  if (!getSinglePhysicalActivation(pr, pc)) {
    calibrationStatus =
      "Capture failed. I need EXACTLY ONE active physical switch. "
      "Remove other magnets and check Sensor Diagnostic.";
    return;
  }

  uint8_t lr = calibrationStep / 8;
  uint8_t lc = calibrationStep % 8;
  uint8_t idx = calibrationStep;

  // The same logical row must always report the same physical row.
  for (uint8_t c = 0; c < 8; c++) {
    uint8_t other = lr * 8 + c;
    if (other == idx || !calibrationCaptured[other]) continue;

    if (calibrationPhysicalRow[other] != pr) {
      calibrationStatus =
        "Mismatch in logical Row " + String(lr + 1) +
        ". This row is detecting more than one physical row. "
        "Put the magnet exactly on the requested square and try again.";
      return;
    }
  }

  // The same logical column must always report the same physical column.
  for (uint8_t r = 0; r < 8; r++) {
    uint8_t other = r * 8 + lc;
    if (other == idx || !calibrationCaptured[other]) continue;

    if (calibrationPhysicalCol[other] != pc) {
      calibrationStatus =
        "Mismatch in logical Column " + String(lc + 1) +
        ". This column is detecting more than one physical column. "
        "Put the magnet exactly on the requested square and try again.";
      return;
    }
  }

  // A physical row cannot belong to two different logical rows.
  for (uint8_t r = 0; r < 8; r++) {
    if (r == lr) continue;
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t other = r * 8 + c;
      if (!calibrationCaptured[other]) continue;
      if (calibrationPhysicalRow[other] == pr) {
        calibrationStatus =
          "Physical Row " + String(pr + 1) +
          " is already assigned to logical Row " + String(r + 1) +
          ". Capture was NOT saved.";
        return;
      }
      break;
    }
  }

  // A physical column cannot belong to two different logical columns.
  for (uint8_t c = 0; c < 8; c++) {
    if (c == lc) continue;
    for (uint8_t r = 0; r < 8; r++) {
      uint8_t other = r * 8 + c;
      if (!calibrationCaptured[other]) continue;
      if (calibrationPhysicalCol[other] == pc) {
        calibrationStatus =
          "Physical Column " + String(pc + 1) +
          " is already assigned to logical Column " + String(c + 1) +
          ". Capture was NOT saved.";
        return;
      }
      break;
    }
  }

  // Save this exact square. Repeated physical rows and columns across
  // their 8 squares are EXPECTED and are not duplicates.
  calibrationCaptured[idx] = true;
  calibrationPhysicalRow[idx] = pr;
  calibrationPhysicalCol[idx] = pc;

  // HARD SAVE after EVERY successful square, including the next step.
  calibrationStatus =
    "Saved " + squareName(lr, lc) +
    " -> Physical Row " + String(pr + 1) +
    ", Physical Column " + String(pc + 1) +
    ".";

  // Keep the live operational mapping unchanged until all 64 squares
  // are verified. This prevents partial calibration from being rejected
  // because the old mapping is still in use for the uncalibrated lines.
  saveCalibrationProgress(true);

  Serial.print("CALIBRATION ");
  Serial.print(squareName(lr, lc));
  Serial.print(" -> physical ROW ");
  Serial.print(pr + 1);
  Serial.print(" GPIO ");
  Serial.print(defaultRowPins[pr]);
  Serial.print(" | COL ");
  Serial.print(pc + 1);
  Serial.print(" GPIO ");
  Serial.println(defaultColPins[pc]);

  if (calibrationStep >= 63) {
    if (!deriveFinalMapping()) {
      calibrationStatus =
        "Calibration finished, but the 64 captured squares are inconsistent. "
        "The final mapping was NOT changed.";
      saveCalibrationProgress(true);
      return;
    }

    // HARD SAVE the final verified mapping.
    saveMapping();
    saveCalibrationProgress(false);
    calibrationRunning = false;
    calibrationSummary = buildCalibrationSummary();

    calibrationStatus =
      "DONE. All 64 squares calibrated, verified, and HARD-SAVED to ESP32 flash.";

    if (pieceTrackingEnabled) updatePieceTrackingLEDs();
    return;
  }

  calibrationStep++;

  String nextSquare = squareName(calibrationStep / 8, calibrationStep % 8);
  calibrationStatus =
    "Saved. Next square: " + nextSquare +
    ". Place ONE magnet there and use the prompt to capture it.";

  // Save again with the new step number so resume goes directly to the next square.
  saveCalibrationProgress(true);
}

// ============================================================
// Count active switches
// ============================================================

uint16_t activeSwitchCount() {
  uint16_t count = 0;

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      if (matrixState[r][c]) count++;
    }
  }

  return count;
}


// ============================================================
// ARCADE GAMES - 11 PHYSICAL BOARD GAMES
// ============================================================
// IMPORTANT SENSOR NOTE:
// Reed switches detect magnet presence, not pressure. A "tap/press"
// action is therefore implemented as a quick LIFT + RETURN on the
// SAME square. This works for shooting, flap, rotate, reveal, etc.
// H8 is currently ACTIVE so the full 8x8 board can be tested.

enum GameMode {
  GAME_NONE,
  GAME_BREAKOUT,
  GAME_SPACE_INVADERS,
  GAME_FROGGER,
  GAME_FLAPPY,
  GAME_PONG,
  GAME_TRON,
  GAME_TANKS,
  GAME_TETRIS,
  GAME_SNAKE,
  GAME_MINESWEEPER,
  GAME_WHACK,
  GAME_LIGHTS_OUT,
  GAME_MAZE_RUNNER,
  GAME_MEMORY_PATH
};

GameMode gameMode = GAME_NONE;
bool gameActive = false;
bool gameHasBaseline = false;
bool gameWaiting = false;
uint8_t gameMovingPlayer = 0; // 1 or 2 for two-piece games
uint8_t gameWaitRow = 0;
uint8_t gameWaitCol = 0;
unsigned long gameWaitSince = 0;

bool gameRejectReturn = false;
uint8_t gameRejectRow = 0, gameRejectCol = 0;
uint8_t gameReturnRow = 0, gameReturnCol = 0;

const unsigned long GAME_TAP_WINDOW_MS = 900;

uint16_t gameScore = 0;
uint16_t gameMoves = 0;
uint8_t gameLives = 0;
String gameStatus = "No game selected.";
uint8_t gameGrid[64];
uint8_t gameTargetRow = 255;
uint8_t gameTargetCol = 255;
unsigned long gameLastTick = 0;

CRGB gameP1Color = CRGB(0, 210, 255);
CRGB gameP2Color = CRGB(255, 80, 180);
CRGB gameTargetColor = CRGB(255, 205, 20);
CRGB gameEnemyColor = CRGB(255, 55, 60);
CRGB gameBallColor = CRGB(255, 255, 255);
CRGB gameWallColor = CRGB(145, 80, 255);
CRGB gameTrailColor = CRGB(30, 170, 90);
CRGB gameGuideColor = CRGB(0, 90, 190);
CRGB gameWarningColor = CRGB(255, 0, 0);
CRGB gameLegalColor = CRGB(255, 210, 48);
CRGB gameLiftColor = CRGB(66, 170, 255);

void loadGameColors() {
  prefs.begin("smartchess", true);
  gameP1Color = unpackRGB(prefs.getUInt("gamep1", packRGB(CRGB(0,210,255))));
  gameP2Color = unpackRGB(prefs.getUInt("gamep2", packRGB(CRGB(255,80,180))));
  gameTargetColor = unpackRGB(prefs.getUInt("gametgt", packRGB(CRGB(255,205,20))));
  gameGuideColor = unpackRGB(prefs.getUInt("gamegui", packRGB(CRGB(0,90,190))));
  gameWarningColor = unpackRGB(prefs.getUInt("gamewrn", packRGB(CRGB(255,0,0))));
  gameLegalColor = unpackRGB(prefs.getUInt("gamlgl", packRGB(CRGB(255,210,48))));
  gameLiftColor = unpackRGB(prefs.getUInt("gmlift", packRGB(CRGB(66,170,255))));
  prefs.end();
}

void saveGameColors() {
  prefs.begin("smartchess", false);
  prefs.putUInt("gamep1", packRGB(gameP1Color));
  prefs.putUInt("gamep2", packRGB(gameP2Color));
  prefs.putUInt("gametgt", packRGB(gameTargetColor));
  prefs.putUInt("gamegui", packRGB(gameGuideColor));
  prefs.putUInt("gamewrn", packRGB(gameWarningColor));
  prefs.putUInt("gamlgl", packRGB(gameLegalColor));
  prefs.putUInt("gmlift", packRGB(gameLiftColor));
  prefs.end();
}

// Generic piece positions.
uint8_t gameP1Row = 0, gameP1Col = 0;
uint8_t gameP2Row = 0, gameP2Col = 7;

// -----------------------------
// Breakout / Arkanoid
// -----------------------------
bool breakoutBricks[64];
uint8_t breakoutBallR = 1, breakoutBallC = 3;
int8_t breakoutDR = 1, breakoutDC = 1;
uint8_t breakoutPaddleC = 3;
uint8_t breakoutBricksLeft = 0;
uint8_t breakoutLives = 3;
unsigned long breakoutTimer = 0;
const unsigned long BREAKOUT_TICK_MS = 260;

// -----------------------------
// Space Invaders / Galaga
// -----------------------------
bool siAliens[64];
uint8_t siShipC = 3;
int8_t siAlienDir = 1;
uint8_t siMissileR = 255, siMissileC = 255;
unsigned long siTimer = 0;
unsigned long siAlienTimer = 0;
uint8_t siWave = 1;
const unsigned long SI_MISSILE_TICK_MS = 150;
const unsigned long SI_ALIEN_TICK_MS = 900;

// -----------------------------
// Frogger
// -----------------------------
uint8_t frogR = 0, frogC = 3;
uint8_t frogCars[8]; // bit mask for rows 1..6
uint8_t frogLaneDir[8];
unsigned long frogTimer = 0;
const unsigned long FROGGER_TICK_MS = 650;

// -----------------------------
// Flappy Bird
// -----------------------------
uint8_t flappyBirdR = 3, flappyBirdC = 3;
uint8_t flappyControlR = 3, flappyControlC = 3;
int8_t flappyVelocity = 0;
int8_t flappyPipeX = 7;
uint8_t flappyGapStart = 3;
uint16_t flappyPassed = 0;
unsigned long flappyTimer = 0;
unsigned long flappyPipeTimer = 0;
const unsigned long FLAPPY_PHYSICS_MS = 430;
const unsigned long FLAPPY_PIPE_MS = 620;

// -----------------------------
// Pong
// -----------------------------
uint8_t pongP1R = 3, pongP2R = 3;
uint8_t pongBallR = 3, pongBallC = 4;
int8_t pongDR = 1, pongDC = -1;
uint8_t pongScore1 = 0, pongScore2 = 0;
unsigned long pongTimer = 0;
const unsigned long PONG_TICK_MS = 280;

// -----------------------------
// Tron / Snake Versus
// -----------------------------
bool tronTrail1[64];
bool tronTrail2[64];
uint8_t tronP1R = 3, tronP1C = 3;
uint8_t tronP2R = 4, tronP2C = 4;
uint8_t tronMoveP1 = 0, tronMoveP2 = 0;

// -----------------------------
// Combat / Tank Battalion
// -----------------------------
bool tankWalls[64];
uint8_t tankP1R = 1, tankP1C = 1;
uint8_t tankP2R = 6, tankP2C = 6;
uint8_t tankBulletR = 255, tankBulletC = 255;
int8_t tankBulletDR = 0, tankBulletDC = 0;
uint8_t tankBulletOwner = 0;
unsigned long tankBulletTimer = 0;
const unsigned long TANK_BULLET_TICK_MS = 150;

// -----------------------------
// Tetris
// -----------------------------
bool tetrisSettled[64];
uint8_t tetrisShape = 0;
uint8_t tetrisRotation = 0;
int8_t tetrisX = 2;
int8_t tetrisY = 4;
uint8_t tetrisControlC = 3;
bool tetrisLiftWaiting = false;
unsigned long tetrisLiftSince = 0;
unsigned long tetrisFallTimer = 0;
uint8_t tetrisLines = 0;
const unsigned long TETRIS_FALL_MS = 520;
const unsigned long TETRIS_HARD_DROP_MS = 700;

// -----------------------------
// Snake Solo
// -----------------------------
uint8_t snakeR[64], snakeC[64];
uint8_t snakeLength = 4;
uint8_t snakeHeadIndex = 0;
uint8_t snakeFoodR = 5, snakeFoodC = 5;
int8_t snakeLastDR = 0, snakeLastDC = 1;

// -----------------------------
// Minesweeper
// -----------------------------
bool minePresent[64];
bool mineRevealed[64];
bool mineFlagged[64];
uint8_t mineCountTotal = 10;
uint8_t mineRevealedSafe = 0;
uint8_t mineCursorR = 0, mineCursorC = 0;
uint8_t mineFlagR = 0, mineFlagC = 1;

// -----------------------------
// Whack-a-Mole
// -----------------------------
uint8_t whackPlayerR = 0, whackPlayerC = 3;
uint8_t whackMoleR = 5, whackMoleC = 5;
uint8_t whackHits = 0;
unsigned long whackStartTime = 0;
unsigned long whackMoleTimer = 0;
const unsigned long WHACK_GAME_MS = 30000;
const unsigned long WHACK_MOLE_MS = 950;

// -----------------------------
// Lights Out
// -----------------------------
bool lightsOutOn[64];
uint8_t lightsOutPlayerR=0, lightsOutPlayerC=0;
uint8_t lightsOutMoves=0;

// -----------------------------
// Maze Runner
// -----------------------------
bool mazeWalls[64];
uint8_t mazePlayerR=0, mazePlayerC=0;
const uint8_t mazeExitR=7, mazeExitC=7;

// -----------------------------
// Memory Path
// -----------------------------
uint8_t memorySequence[24];
uint8_t memoryLength=4, memoryStep=0, memoryPlayIndex=0;
uint8_t memoryLives=3;
bool memoryPlaying=false;
unsigned long memoryTimer=0;
const unsigned long MEMORY_FLASH_MS=520;

void gameProcessBoardChange();
void gameUpdate();
void gameRenderLEDs();

bool gameIsH8(uint8_t r, uint8_t c) {
  // H8 is kept ACTIVE for now.
  return false;
}

bool gameSameSquare(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2) {
  return r1 == r2 && c1 == c2;
}

bool gameValidSquare(int r, int c) {
  return r >= 0 && r < 8 && c >= 0 && c < 8 && !gameIsH8((uint8_t)r, (uint8_t)c);
}

int gameManhattan(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2) {
  return abs((int)r1 - (int)r2) + abs((int)c1 - (int)c2);
}

bool gameCardinalOneStep(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2) {
  return gameManhattan(r1, c1, r2, c2) == 1;
}

void gameClearGrid() {
  memset(gameGrid, 0, sizeof(gameGrid));
  gameTargetRow = 255;
  gameTargetCol = 255;
}

void gameGridSet(uint8_t r, uint8_t c, uint8_t code) {
  if (!gameValidSquare(r, c)) return;
  gameGrid[r * 8 + c] = code;
}

String gameModeName() {
  switch (gameMode) {
    case GAME_BREAKOUT: return "Breakout";
    case GAME_SPACE_INVADERS: return "Space Invaders";
    case GAME_FROGGER: return "Frogger";
    case GAME_FLAPPY: return "Flappy Bird";
    case GAME_PONG: return "Pong";
    case GAME_TRON: return "Tron Versus";
    case GAME_TANKS: return "Combat Tanks";
    case GAME_TETRIS: return "Tetris";
    case GAME_SNAKE: return "Snake Solo";
    case GAME_MINESWEEPER: return "Minesweeper";
    case GAME_WHACK: return "Whack-a-Mole";
    case GAME_LIGHTS_OUT: return "Lights Out";
    case GAME_MAZE_RUNNER: return "Maze Runner";
    case GAME_MEMORY_PATH: return "Memory Path";
    default: return "None";
  }
}

String gameTargetName() {
  if (gameTargetRow == 255 || gameTargetCol == 255) return "--";
  return squareName(gameTargetRow, gameTargetCol);
}

bool gameCanMoveTo(uint8_t player, uint8_t fromR, uint8_t fromC, int toR, int toC) {
  if (!gameValidSquare(toR, toC)) return false;
  uint8_t tr = (uint8_t)toR, tc = (uint8_t)toC;

  if (gameMode == GAME_BREAKOUT || gameMode == GAME_SPACE_INVADERS || gameMode == GAME_TETRIS) {
    return tr == 0 && abs((int)tc - (int)fromC) == 1;
  }

  if (gameMode == GAME_PONG) {
    if (player == 1) return tc == 0 && abs((int)tr - (int)fromR) == 1;
    return tc == 7 && abs((int)tr - (int)fromR) == 1;
  }

  if (gameMode == GAME_TANKS) {
    if (!gameCardinalOneStep(fromR, fromC, tr, tc)) return false;
    if (tankWalls[tr * 8 + tc]) return false;
    uint8_t orow, ocol; gameGetTwoPiecePosition(player == 1 ? 2 : 1, orow, ocol);
    if (gameSameSquare(tr, tc, orow, ocol)) return false;
    return true;
  }

  if (gameMode == GAME_TRON) {
    if (!gameCardinalOneStep(fromR, fromC, tr, tc)) return false;
    if (player == 1 && (tronTrail1[tr*8+tc] || tronTrail2[tr*8+tc])) return false;
    if (player == 2 && (tronTrail1[tr*8+tc] || tronTrail2[tr*8+tc])) return false;
    uint8_t orow, ocol; gameGetTwoPiecePosition(player == 1 ? 2 : 1, orow, ocol);
    if (gameSameSquare(tr, tc, orow, ocol)) return false;
    return true;
  }

  if (gameMode == GAME_MAZE_RUNNER) {
    if (!gameCardinalOneStep(fromR,fromC,tr,tc)) return false;
    return !mazeWalls[tr*8+tc];
  }

  if (gameMode == GAME_MINESWEEPER) {
    if (!gameCardinalOneStep(fromR, fromC, tr, tc)) return false;
    if (player == 1 && mineFlagged[tr * 8 + tc]) return false;
    if (player == 2 && gameSameSquare(tr, tc, mineCursorR, mineCursorC)) return false;
    return true;
  }

  // Frogger, Flappy Bird, Snake and Whack-a-Mole: one cardinal square at a time.
  if (gameCardinalOneStep(fromR, fromC, tr, tc)) return true;
  return false;
}

void gameShowGuides(uint8_t player, uint8_t fromR, uint8_t fromC) {
  // Guides are legal destination squares only. The square under a stationary
  // piece stays colored as a piece; while the piece is lifted, its old square is cleared.
  for (int dr = -1; dr <= 1; dr++) {
    for (int dc = -1; dc <= 1; dc++) {
      if (dr == 0 && dc == 0) continue;
      int tr = (int)fromR + dr, tc = (int)fromC + dc;
      if (gameCanMoveTo(player, fromR, fromC, tr, tc)) gameGridSet((uint8_t)tr, (uint8_t)tc, 10);
    }
  }
}

void gameOverlayMoveGuides() {
  if (!gameWaiting && !gameRejectReturn) return;

  uint8_t guidePlayer = gameMovingPlayer ? gameMovingPlayer : 1;
  uint8_t r = gameWaitRow, c = gameWaitCol;
  gameGrid[r * 8 + c] = 0;
  gameShowGuides(guidePlayer, r, c);

  if (gameRejectReturn) {
    gameGridSet(gameRejectRow, gameRejectCol, 8);
    gameGridSet(gameReturnRow, gameReturnCol, 10);
  }
}

void gameRenderLEDs() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  if (gameMode == GAME_NONE) {
    FastLED.show();
    return;
  }

  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t code = gameGrid[r * 8 + c];
      CRGB color = CRGB::Black;
      switch (code) {
        case 1: color = gameP1Color; break;
        case 2: color = gameP2Color; break;
        case 3: color = gameTargetColor; break;
        case 4: color = gameEnemyColor; break;
        case 5: color = gameBallColor; break;
        case 6: color = CRGB(170, 80, 255); break;
        case 7: color = gameTrailColor; break;
        case 8: color = gameWarningColor; break;
        case 9: color = CRGB(255, 130, 0); break;
        case 10: color = gameGuideColor; break;
        case 11: color = CRGB(0, 180, 255); break;
        case 12: color = CRGB(255, 255, 80); break;
        default:
          if (code >= 20 && code <= 28) {
            uint8_t n = code - 20;
            color = (n == 0) ? CRGB(15, 30, 45) : CRGB(20 + n * 18, 40 + n * 18, 100);
          }
          break;
      }
      leds[chessToLED(r, c)] = color;
    }
  }

  // The guides are rendered last so they are always visible while a piece is lifted.
  gameOverlayMoveGuides();
  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t code = gameGrid[r * 8 + c];
      if (code == 10) leds[chessToLED(r,c)] = gameGuideColor;
      else if (code == 8) leds[chessToLED(r,c)] = gameWarningColor;
    }
  }
  FastLED.show();
}

uint16_t gameActiveCountFromMatrix() {
  return activeSwitchCount();
}

bool gameFindSingle(uint8_t &r, uint8_t &c) {
  if (gameActiveCountFromMatrix() != 1) return false;
  for (uint8_t rr = 0; rr < 8; rr++) {
    for (uint8_t cc = 0; cc < 8; cc++) {
      if (matrixState[rr][cc]) {
        r = rr;
        c = cc;
        return true;
      }
    }
  }
  return false;
}

bool gameFindTwo(uint8_t &r1, uint8_t &c1, uint8_t &r2, uint8_t &c2) {
  if (gameActiveCountFromMatrix() != 2) return false;
  bool found1 = false;
  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      if (!matrixState[r][c]) continue;
      if (!found1) {
        r1 = r; c1 = c; found1 = true;
      } else {
        r2 = r; c2 = c; return true;
      }
    }
  }
  return false;
}

void gameStartMessage(const String &msg) {
  gameStatus = msg;
  gameRenderLEDs();
}

void gameStop() {
  gameActive = false;
  gameHasBaseline = false;
  gameWaiting = false;
  gameMovingPlayer = 0;
  gameRejectReturn = false;
  gameMode = GAME_NONE;
  gameClearGrid();
  clearLEDs();
  if (pieceTrackingEnabled) updatePieceTrackingLEDs();
  gameStatus = "Game stopped.";
}

bool gameRequireSingle(bool bottomRow, uint8_t &r, uint8_t &c) {
  if (!gameFindSingle(r, c)) {
    gameActive = false;
    gameHasBaseline = false;
    gameStatus = bottomRow ? "Start failed: place exactly ONE magnet on Rank 1." : "Start failed: place exactly ONE magnet.";
    clearLEDs();
    return false;
  }
  if (gameIsH8(r, c)) {
    gameActive = false;
    gameHasBaseline = false;
    gameStatus = "H8 is active.";
    clearLEDs();
    return false;
  }
  if (bottomRow && r != 0) {
    gameActive = false;
    gameHasBaseline = false;
    gameStatus = "Start failed: this game uses the bottom row (Rank 1) as its controller.";
    clearLEDs();
    return false;
  }
  return true;
}

bool gameRequireTwo(uint8_t &r1, uint8_t &c1, uint8_t &r2, uint8_t &c2) {
  if (!gameFindTwo(r1, c1, r2, c2)) {
    gameActive = false;
    gameHasBaseline = false;
    gameStatus = "Start failed: place exactly TWO magnets.";
    clearLEDs();
    return false;
  }
  if (gameIsH8(r1, c1) || gameIsH8(r2, c2)) {
    gameActive = false;
    gameHasBaseline = false;
    gameStatus = "H8 is active.";
    clearLEDs();
    return false;
  }
  return true;
}

// ============================================================
// Breakout
// ============================================================
void breakoutBuildBricks() {
  memset(breakoutBricks, 0, sizeof(breakoutBricks));
  breakoutBricksLeft = 0;
  for (uint8_t r = 5; r <= 7; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      if (gameIsH8(r, c)) continue;
      breakoutBricks[r * 8 + c] = true;
      breakoutBricksLeft++;
    }
  }
}

void breakoutResetBall() {
  breakoutBallR = 2;
  breakoutBallC = breakoutPaddleC;
  breakoutDR = 1;
  breakoutDC = random(0, 2) ? 1 : -1;
}

void breakoutRender() {
  gameClearGrid();
  for (uint8_t i = 0; i < 64; i++) if (breakoutBricks[i]) gameGrid[i] = 6;
  gameGridSet(0, breakoutPaddleC, 11);
  gameGridSet(breakoutBallR, breakoutBallC, 5);
}

void breakoutMiss() {
  if (breakoutLives > 0) breakoutLives--;
  gameLives = breakoutLives;
  if (breakoutLives == 0) {
    gameActive = false;
    gameStatus = "BREAKOUT GAME OVER - all lives used. Press START again.";
    FastLED.show();
    return;
  }
  breakoutResetBall();
  gameStatus = "MISS! Lives left: " + String(breakoutLives) + ". Keep the paddle under the ball.";
}

void breakoutTick() {
  if (millis() - breakoutTimer < BREAKOUT_TICK_MS) return;
  breakoutTimer = millis();

  int nr = (int)breakoutBallR + breakoutDR;
  int nc = (int)breakoutBallC + breakoutDC;

  if (nc < 0 || nc > 7) {
    breakoutDC = -breakoutDC;
    nc = (int)breakoutBallC + breakoutDC;
  }

  if (nr > 7) {
    breakoutDR = -1;
    nr = (int)breakoutBallR - 1;
  }

  if (nr <= 0) {
    nr = 0;
    if (nc == breakoutPaddleC) {
      breakoutDR = 1;
      gameScore++;
      gameStatus = "BOUNCE! Score: " + String(gameScore);
      nr = 1;
    } else {
      breakoutMiss();
      breakoutRender();
      gameRenderLEDs();
      return;
    }
  }

  if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8 && breakoutBricks[nr * 8 + nc]) {
    breakoutBricks[nr * 8 + nc] = false;
    if (breakoutBricksLeft) breakoutBricksLeft--;
    gameScore++;
    breakoutDR = -breakoutDR;
    nr = (int)breakoutBallR + breakoutDR;
    if (breakoutBricksLeft == 0) {
      gameActive = false;
      gameStatus = "BREAKOUT COMPLETE! Score: " + String(gameScore) + ". Press START again for a new board.";
    }
  }

  breakoutBallR = constrain(nr, 0, 7);
  breakoutBallC = constrain(nc, 0, 7);
  breakoutRender();
  gameRenderLEDs();
}

void startBreakout(uint8_t r, uint8_t c) {
  breakoutPaddleC = c;
  gameP1Row=0; gameP1Col=c;
  breakoutLives = 3;
  gameLives = 3;
  gameScore = 0;
  gameMoves = 0;
  randomSeed(micros());
  breakoutBuildBricks();
  breakoutResetBall();
  breakoutTimer = millis();
  gameMode = GAME_BREAKOUT;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Breakout: keep the Rank 1 paddle under the ball. Move the magnet left/right.";
  breakoutRender();
  gameRenderLEDs();
}

// ============================================================
// Space Invaders
// ============================================================
void spaceBuildAliens() {
  memset(siAliens, 0, sizeof(siAliens));
  for (uint8_t r = 5; r <= 7; r++) {
    for (uint8_t c = 1; c <= 6; c++) siAliens[r * 8 + c] = true;
  }
  siMissileR = 255;
  siMissileC = 255;
  siAlienDir = 1;
  siWave = 1;
}

void spaceRender() {
  gameClearGrid();
  for (uint8_t i = 0; i < 64; i++) if (siAliens[i]) gameGrid[i] = 4;
  if (siMissileR != 255) gameGridSet(siMissileR, siMissileC, 5);
  gameGridSet(0, siShipC, 1);
}

void spaceFire() {
  if (siMissileR != 255) {
    gameStatus = "MISSILE ALREADY IN FLIGHT.";
    return;
  }
  if (siShipC < 8) {
    siMissileR = 1;
    siMissileC = siShipC;
    gameStatus = "FIRE! Move or tap again when the missile is gone.";
  }
}

void spaceTick() {
  unsigned long now = millis();

  if (siMissileR != 255 && now - siTimer >= SI_MISSILE_TICK_MS) {
    siTimer = now;
    int nr = (int)siMissileR + 1;
    if (nr > 7) {
      siMissileR = 255;
    } else if (siAliens[nr * 8 + siMissileC]) {
      siAliens[nr * 8 + siMissileC] = false;
      siMissileR = 255;
      gameScore++;
      gameStatus = "ALIEN HIT! Score: " + String(gameScore);
      bool any = false;
      for (uint8_t i = 0; i < 64; i++) if (siAliens[i]) { any = true; break; }
      if (!any) {
        gameActive = false;
        gameStatus = "SPACE INVADERS WAVE CLEARED! Score: " + String(gameScore);
      }
    } else {
      siMissileR = (uint8_t)nr;
    }
  }

  if (gameActive && now - siAlienTimer >= SI_ALIEN_TICK_MS) {
    siAlienTimer = now;
    bool hitEdge = false;
    for (uint8_t r = 0; r < 8; r++) {
      if ((siAlienDir < 0 && siAliens[r * 8 + 0]) ||
          (siAlienDir > 0 && siAliens[r * 8 + 6])) {
        hitEdge = true;
        break;
      }
    }

    if (hitEdge) {
      int targetShift = -1;
      for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
          if (siAliens[r * 8 + c]) {
            if (r + targetShift < 0) {
              gameActive = false;
              gameStatus = "ALIENS REACHED YOUR SIDE - GAME OVER.";
              break;
            }
          }
        }
        if (!gameActive) break;
      }
      if (gameActive) {
        bool nextAliens[64];
        memset(nextAliens, 0, sizeof(nextAliens));
        for (int r = 0; r < 8; r++) {
          for (int c = 0; c < 8; c++) if (siAliens[r * 8 + c]) {
            int nr = r - 1;
            if (nr < 0) {
              gameActive = false;
              break;
            }
            nextAliens[nr * 8 + c] = true;
          }
          if (!gameActive) break;
        }
        if (gameActive) memcpy(siAliens, nextAliens, sizeof(siAliens));
      }
      siAlienDir = -siAlienDir;
    } else {
      bool nextAliens[64];
      memset(nextAliens, 0, sizeof(nextAliens));
      for (uint8_t r = 0; r < 8; r++) for (uint8_t c = 0; c < 8; c++) if (siAliens[r * 8 + c]) {
        int nc = (int)c + siAlienDir;
        if (nc >= 0 && nc < 8 && !gameIsH8(r, (uint8_t)nc)) nextAliens[r * 8 + nc] = true;
      }
      memcpy(siAliens, nextAliens, sizeof(siAliens));
    }

    for (uint8_t c = 0; c < 8; c++) {
      if (siAliens[c] && c == siShipC) {
        gameActive = false;
        gameStatus = "ALIENS LANDED ON THE SHIP - GAME OVER.";
        break;
      }
    }
  }

  spaceRender();
  gameRenderLEDs();
}

void startSpaceInvaders(uint8_t r, uint8_t c) {
  siShipC = c; gameP1Row=0; gameP1Col=c;
  gameScore = 0;
  gameMoves = 0;
  spaceBuildAliens();
  siTimer = millis();
  siAlienTimer = millis();
  gameMode = GAME_SPACE_INVADERS;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Space Invaders: move on Rank 1. Tap = lift and return on the same square to fire.";
  spaceRender();
  gameRenderLEDs();
}

// ============================================================
// Frogger
// ============================================================
void frogBuildLanes() {
  memset(frogCars, 0, sizeof(frogCars));
  for (uint8_t r = 1; r <= 6; r++) {
    frogLaneDir[r] = (r % 2) ? 1 : -1;
    frogCars[r] = (r % 2) ? 0b00010001 : 0b00100100;
  }
}

void frogRender() {
  gameClearGrid();
  for (uint8_t r = 1; r <= 6; r++) for (uint8_t c = 0; c < 8; c++) if (frogCars[r] & (1u << c)) gameGridSet(r, c, 4);
  gameGridSet(frogR, frogC, 1);
  gameTargetRow = 7; gameTargetCol = frogC;
  gameGridSet(7, frogC, 3);
  gameGridSet(frogR, frogC, 1);
}

bool frogHit() {
  return (frogCars[frogR] & (1u << frogC)) != 0;
}

void frogTick() {
  if (millis() - frogTimer < FROGGER_TICK_MS) return;
  frogTimer = millis();
  for (uint8_t r = 1; r <= 6; r++) {
    uint8_t bits = frogCars[r];
    if (frogLaneDir[r] > 0) frogCars[r] = (uint8_t)((bits << 1) | (bits >> 7));
    else frogCars[r] = (uint8_t)((bits >> 1) | (bits << 7));
  }
  if (frogHit()) {
    gameLives = (gameLives > 0) ? gameLives - 1 : 0;
    if (gameLives == 0) {
      gameActive = false;
      gameStatus = "FROGGER GAME OVER. Press START again.";
    } else {
      frogR = 0;
      frogC = 3;
      gameStatus = "SPLAT! Lives left: " + String(gameLives);
    }
  }
  frogRender();
  gameRenderLEDs();
}

void startFrogger(uint8_t r, uint8_t c) {
  frogR = r; frogC = c;
  if (frogR != 0) frogR = 0;
  gameP1Row=frogR; gameP1Col=frogC;
  gameScore = 0; gameMoves = 0; gameLives = 3;
  frogBuildLanes();
  frogTimer = millis();
  gameMode = GAME_FROGGER;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Frogger: move ONE square up/down/left/right and reach Rank 8. Avoid the moving cars.";
  frogRender();
  gameRenderLEDs();
}

// ============================================================
// Flappy Bird
// ============================================================
void flappyNewPipe() {
  flappyPipeX = 7;
  flappyGapStart = random(1, 6);
}

void flappyRender() {
  gameClearGrid();
  if (flappyPipeX >= 0 && flappyPipeX < 8) {
    for (uint8_t r = 0; r < 8; r++) {
      if (!(r == flappyGapStart || r == flappyGapStart + 1)) gameGridSet(r, (uint8_t)flappyPipeX, 4);
    }
  }
  gameGridSet(flappyControlR, flappyControlC, 10);
  gameGridSet(flappyBirdR, flappyBirdC, 1);
  gameTargetRow = flappyBirdR; gameTargetCol = flappyBirdC;
}

void flappyTap() {
  flappyVelocity = 2;
  gameStatus = "FLAP! Keep tapping to stay in the gap.";
}

void flappyTick() {
  unsigned long now = millis();
  if (now - flappyTimer >= FLAPPY_PHYSICS_MS) {
    flappyTimer = now;
    flappyBirdR = (uint8_t)constrain((int)flappyBirdR + (flappyVelocity > 0 ? 1 : (flappyVelocity < 0 ? -1 : 0)), 0, 7);
    if (flappyVelocity > -3) flappyVelocity--;
    if (flappyBirdR == 0 || flappyBirdR == 7) {
      gameActive = false;
      gameStatus = "FLAPPY CRASHED - Score: " + String(gameScore);
    }
  }

  if (gameActive && now - flappyPipeTimer >= FLAPPY_PIPE_MS) {
    flappyPipeTimer = now;
    if (flappyPipeX <= 0) {
      flappyPipeX = 7;
      flappyGapStart = random(1, 6);
      flappyPassed++;
      gameScore = flappyPassed;
      gameStatus = "Pipe cleared! Score: " + String(gameScore);
    } else {
      flappyPipeX--;
    }
  }

  if (gameActive && flappyPipeX == flappyBirdC &&
      !(flappyBirdR == flappyGapStart || flappyBirdR == flappyGapStart + 1)) {
    gameActive = false;
    gameStatus = "YOU HIT THE PIPE - Score: " + String(gameScore);
  }
  flappyRender();
  gameRenderLEDs();
}

void startFlappy(uint8_t r, uint8_t c) {
  flappyControlR = r;
  flappyControlC = c;
  gameP1Row=r; gameP1Col=c;
  flappyBirdR = constrain((int)r, 1, 6);
  flappyBirdC = c;
  flappyVelocity = 0;
  flappyPassed = 0;
  gameScore = 0; gameMoves = 0;
  flappyNewPipe();
  flappyTimer = millis();
  flappyPipeTimer = millis();
  gameMode = GAME_FLAPPY;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Flappy Bird: tap = lift and return the magnet on the SAME square. Move the magnet to change columns.";
  flappyRender();
  gameRenderLEDs();
}

// ============================================================
// Pong
// ============================================================
void pongResetBall(int8_t direction) {
  pongBallR = 3;
  pongBallC = direction < 0 ? 4 : 3;
  pongDR = random(0, 2) ? 1 : -1;
  pongDC = direction;
}

void pongRender() {
  gameClearGrid();
  gameGridSet(pongP1R, 0, 1);
  if (!gameIsH8(pongP2R, 7)) gameGridSet(pongP2R, 7, 2);
  gameGridSet(pongBallR, pongBallC, 5);
}

void pongTick() {
  if (millis() - pongTimer < PONG_TICK_MS) return;
  pongTimer = millis();

  int nr = (int)pongBallR + pongDR;
  int nc = (int)pongBallC + pongDC;
  if (nr < 0 || nr > 7) { pongDR = -pongDR; nr = (int)pongBallR + pongDR; }

  if (nc < 0) {
    if (abs((int)pongBallR - (int)pongP1R) <= 1) {
      pongDC = 1;
      nc = 1;
      gameScore++;
      gameStatus = "P1 RETURN! Total hits: " + String(gameScore);
    } else {
      pongScore2++;
      gameStatus = "P2 SCORES. P2: " + String(pongScore2) + "  P1: " + String(pongScore1);
      if (pongScore2 >= 5) { gameActive = false; gameStatus += " - P2 wins the match."; }
      pongResetBall(1);
      pongRender();
      gameRenderLEDs();
      return;
    }
  }

  if (nc > 7) {
    if (abs((int)pongBallR - (int)pongP2R) <= 1) {
      pongDC = -1;
      nc = 6;
      gameScore++;
      gameStatus = "P2 RETURN! Total hits: " + String(gameScore);
    } else {
      pongScore1++;
      gameStatus = "P1 SCORES. P2: " + String(pongScore2) + "  P1: " + String(pongScore1);
      if (pongScore1 >= 5) { gameActive = false; gameStatus += " - P1 wins the match."; }
      pongResetBall(-1);
      pongRender();
      gameRenderLEDs();
      return;
    }
  }

  pongBallR = constrain(nr, 0, 7);
  pongBallC = constrain(nc, 0, 7);
  pongRender();
  gameRenderLEDs();
}

void startPong(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2) {
  if (c1 != 0 || c2 != 7) {
    gameActive = false;
    gameHasBaseline = false;
    gameStatus = "PONG START: P1 must be on A-file and P2 on H-file.";
    clearLEDs();
    return;
  }
  pongP1R = r1; pongP2R = r2;
  gameP1Row=r1; gameP1Col=0; gameP2Row=r2; gameP2Col=7;
  pongScore1 = 0; pongScore2 = 0;
  gameScore = 0; gameMoves = 0;
  pongResetBall(-1);
  pongTimer = millis();
  gameMode = GAME_PONG;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Pong: P1 moves on A-file, P2 on H-file. Intercept the ball.";
  pongRender();
  gameRenderLEDs();
}

// ============================================================
// Tron Versus
// ============================================================
void tronReset() {
  memset(tronTrail1, 0, sizeof(tronTrail1));
  memset(tronTrail2, 0, sizeof(tronTrail2));
  tronP1R = 3; tronP1C = 3;
  tronP2R = 4; tronP2C = 4;
  tronTrail1[tronP1R * 8 + tronP1C] = true;
  tronTrail2[tronP2R * 8 + tronP2C] = true;
}

void tronRender() {
  gameClearGrid();
  for (uint8_t i = 0; i < 64; i++) if (tronTrail1[i] || tronTrail2[i]) gameGrid[i] = 7;
  gameGridSet(tronP1R, tronP1C, 1);
  gameGridSet(tronP2R, tronP2C, 2);
}

void tronCrash(uint8_t player) {
  gameActive = false;
  gameStatus = player == 1 ? "TRON: P1 crashed. P2 wins." : "TRON: P2 crashed. P1 wins.";
  tronRender();
}

void tronApplyMove(uint8_t player, uint8_t nr, uint8_t nc) {
  bool crash = false;
  if (!gameValidSquare(nr, nc)) crash = true;
  if (!crash && (tronTrail1[nr * 8 + nc] || tronTrail2[nr * 8 + nc])) crash = true;
  if (crash) { tronCrash(player); return; }

  if (player == 1) {
    tronTrail1[tronP1R * 8 + tronP1C] = true;
    tronP1R = nr; tronP1C = nc;
    tronTrail1[nr * 8 + nc] = true;
  } else {
    tronTrail2[tronP2R * 8 + tronP2C] = true;
    tronP2R = nr; tronP2C = nc;
    tronTrail2[nr * 8 + nc] = true;
  }
  gameMoves++;
  gameStatus = "Keep moving one square at a time. Avoid walls and all trails.";
  tronRender();
  gameRenderLEDs();
}

void startTron(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2) {
  if (r1 != 3 || c1 != 3 || r2 != 4 || c2 != 4) {
    gameActive = false;
    gameHasBaseline = false;
    gameStatus = "TRON START: place P1 on D4 and P2 on E5 (the center).";
    clearLEDs();
    return;
  }
  gameScore = 0; gameMoves = 0;
  tronReset();
  gameP1Row=tronP1R; gameP1Col=tronP1C; gameP2Row=tronP2R; gameP2Col=tronP2C;
  gameMode = GAME_TRON;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Tron Versus: each move leaves a permanent digital trail. Do not hit walls or trails.";
  tronRender();
  gameRenderLEDs();
}

// ============================================================
// Tank Combat
// ============================================================
void tankBuildMaze() {
  memset(tankWalls, 0, sizeof(tankWalls));
  const uint8_t walls[][2] = {
    {1,3},{1,4},{2,3},{4,3},{4,4},{5,3},{2,5},{3,5},{4,5},{3,2},{5,1}
  };
  for (uint8_t i = 0; i < sizeof(walls) / sizeof(walls[0]); i++) tankWalls[walls[i][0] * 8 + walls[i][1]] = true;
}

void tankRender() {
  gameClearGrid();
  for (uint8_t i = 0; i < 64; i++) if (tankWalls[i]) gameGrid[i] = 6;
  if (tankBulletR != 255) gameGridSet(tankBulletR, tankBulletC, 5);
  gameGridSet(tankP1R, tankP1C, 1);
  gameGridSet(tankP2R, tankP2C, 2);
}

void tankFire(uint8_t owner) {
  if (tankBulletR != 255) {
    gameStatus = "ONE SHELL IS ALREADY IN FLIGHT.";
    return;
  }
  uint8_t r = owner == 1 ? tankP1R : tankP2R;
  uint8_t c = owner == 1 ? tankP1C : tankP2C;
  uint8_t targetR = owner == 1 ? tankP2R : tankP1R;
  uint8_t targetC = owner == 1 ? tankP2C : tankP1C;

  tankBulletR = r;
  tankBulletC = c;
  tankBulletOwner = owner;
  if (targetR < r) tankBulletDR = -1;
  else if (targetR > r) tankBulletDR = 1;
  else tankBulletDR = 0;
  if (targetC < c) tankBulletDC = -1;
  else if (targetC > c) tankBulletDC = 1;
  else tankBulletDC = 0;
  if (tankBulletDR == 0 && tankBulletDC == 0) tankBulletDC = owner == 1 ? 1 : -1;
  gameStatus = owner == 1 ? "P1 FIRED." : "P2 FIRED.";
}

void tankTick() {
  if (tankBulletR == 255 || millis() - tankBulletTimer < TANK_BULLET_TICK_MS) return;
  tankBulletTimer = millis();
  int nr = (int)tankBulletR + tankBulletDR;
  int nc = (int)tankBulletC + tankBulletDC;
  if (!gameValidSquare(nr, nc) || tankWalls[nr * 8 + nc]) {
    tankBulletR = 255;
    tankRender();
    gameRenderLEDs();
    return;
  }
  uint8_t enemyR = tankBulletOwner == 1 ? tankP2R : tankP1R;
  uint8_t enemyC = tankBulletOwner == 1 ? tankP2C : tankP1C;
  if (nr == enemyR && nc == enemyC) {
    gameScore += 3;
    gameActive = false;
    gameStatus = tankBulletOwner == 1 ? "P1 HIT P2 - P1 wins!" : "P2 HIT P1 - P2 wins!";
    tankBulletR = 255;
    tankRender();
    return;
  }
  tankBulletR = (uint8_t)nr;
  tankBulletC = (uint8_t)nc;
  tankRender();
  gameRenderLEDs();
}

void startTanks(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2) {
  if (gameSameSquare(r1, c1, r2, c2)) {
    gameActive = false; gameHasBaseline = false;
    gameStatus = "TANKS START: use TWO different squares.";
    clearLEDs(); return;
  }
  tankP1R = r1; tankP1C = c1; tankP2R = r2; tankP2C = c2;
  gameP1Row=r1; gameP1Col=c1; gameP2Row=r2; gameP2Col=c2;
  tankBuildMaze();
  // Force the tanks out of maze walls if the chosen positions were walls.
  if (tankWalls[tankP1R * 8 + tankP1C] || tankWalls[tankP2R * 8 + tankP2C]) {
    gameActive = false; gameHasBaseline = false;
    gameStatus = "TANKS START: do not place a tank on a purple wall square.";
    clearLEDs(); return;
  }
  tankBulletR = 255;
  gameScore = 0; gameMoves = 0;
  gameMode = GAME_TANKS;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Combat Tanks: move one square at a time through the maze. Tap your tank to fire.";
  tankRender();
  gameRenderLEDs();
}

// ============================================================
// Tetris
// ============================================================
const int8_t TETRIS_BASE[5][4][2] = {
  {{0,1},{1,1},{2,1},{3,1}},
  {{1,0},{2,0},{1,1},{2,1}},
  {{1,0},{0,1},{1,1},{2,1}},
  {{0,0},{0,1},{1,1},{2,1}},
  {{1,0},{2,0},{0,1},{1,1}}
};

void tetrisCell(uint8_t shape, uint8_t rot, uint8_t idx, int8_t &x, int8_t &y) {
  x = TETRIS_BASE[shape % 5][idx][0];
  y = TETRIS_BASE[shape % 5][idx][1];
  for (uint8_t k = 0; k < rot; k++) {
    int8_t nx = 3 - y;
    int8_t ny = x;
    x = nx; y = ny;
  }
}

bool tetrisFits(int8_t px, int8_t py, uint8_t rot, bool includeSettled) {
  for (uint8_t i = 0; i < 4; i++) {
    int8_t lx, ly;
    tetrisCell(tetrisShape, rot, i, lx, ly);
    int wx = px + lx;
    int wy = py + ly;
    if (!gameValidSquare(wy, wx)) return false;
    if (includeSettled && tetrisSettled[wy * 8 + wx]) return false;
  }
  return true;
}

void tetrisRender() {
  gameClearGrid();
  for (uint8_t i = 0; i < 64; i++) if (tetrisSettled[i]) gameGrid[i] = 7;
  for (uint8_t i = 0; i < 4; i++) {
    int8_t lx, ly; tetrisCell(tetrisShape, tetrisRotation, i, lx, ly);
    gameGridSet(tetrisY + ly, tetrisX + lx, 6);
  }
  gameGridSet(0, tetrisControlC, 11);
}

void tetrisSpawn() {
  tetrisShape = random(0, 5);
  tetrisRotation = 0;
  tetrisX = (int8_t)constrain((int)tetrisControlC - 1, 0, 4);
  tetrisY = 3;
  if (!tetrisFits(tetrisX, tetrisY, tetrisRotation, true)) {
    gameActive = false;
    gameStatus = "TETRIS GAME OVER - board is full. Lines: " + String(tetrisLines);
  }
}

void tetrisClearLines() {
  for (int r = 0; r < 8; r++) {
    bool full = true;
    for (int c = 0; c < 8; c++) {
      if (gameIsH8(r, c) || !tetrisSettled[r * 8 + c]) { full = false; break; }
    }
    if (full) {
      for (int rr = r; rr < 7; rr++) for (int c = 0; c < 8; c++) tetrisSettled[rr * 8 + c] = tetrisSettled[(rr + 1) * 8 + c];
      for (int c = 0; c < 8; c++) tetrisSettled[7 * 8 + c] = false;
      tetrisLines++;
      gameScore += 10;
    }
  }
}

void tetrisLockAndSpawn() {
  for (uint8_t i = 0; i < 4; i++) {
    int8_t lx, ly; tetrisCell(tetrisShape, tetrisRotation, i, lx, ly);
    int wx = tetrisX + lx, wy = tetrisY + ly;
    if (gameValidSquare(wy, wx)) tetrisSettled[wy * 8 + wx] = true;
  }
  tetrisClearLines();
  tetrisSpawn();
}

void tetrisRotate() {
  uint8_t nr = (tetrisRotation + 1) % 4;
  if (tetrisFits(tetrisX, tetrisY, nr, true)) tetrisRotation = nr;
  else if (tetrisFits(tetrisX - 1, tetrisY, nr, true)) { tetrisX--; tetrisRotation = nr; }
  else if (tetrisFits(tetrisX + 1, tetrisY, nr, true)) { tetrisX++; tetrisRotation = nr; }
  else { gameStatus = "ROTATE BLOCKED."; return; }
  gameMoves++;
  gameStatus = "ROTATED. Use the Rank 1 magnet to control the column.";
}

void tetrisMoveController(uint8_t c) {
  tetrisControlC = c;
  int8_t desiredX = (int8_t)constrain((int)c - 1, 0, 4);
  if (tetrisFits(desiredX, tetrisY, tetrisRotation, true)) tetrisX = desiredX;
  gameMoves++;
  gameStatus = "Block column: " + String((char)('A' + c)) + ". Lift and hold off-board for hard drop.";
}

void tetrisHardDrop() {
  if (!gameActive) return;
  while (tetrisFits(tetrisX, tetrisY - 1, tetrisRotation, true)) tetrisY--;
  tetrisLockAndSpawn();
  tetrisLiftWaiting = false;
  gameScore++;
  gameStatus = "HARD DROP! Score: " + String(gameScore) + ".";
}

void tetrisTick() {
  if (!gameActive) return;
  if (millis() - tetrisFallTimer < TETRIS_FALL_MS) return;
  tetrisFallTimer = millis();
  if (tetrisFits(tetrisX, tetrisY - 1, tetrisRotation, true)) {
    tetrisY--;
  } else {
    tetrisLockAndSpawn();
  }
  tetrisRender();
  gameRenderLEDs();
}

void startTetris(uint8_t r, uint8_t c) {
  memset(tetrisSettled, 0, sizeof(tetrisSettled));
  tetrisControlC = c; gameP1Row=0; gameP1Col=c;
  tetrisLines = 0;
  gameScore = 0; gameMoves = 0;
  tetrisLiftWaiting = false;
  tetrisFallTimer = millis();
  gameMode = GAME_TETRIS;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  randomSeed(micros());
  tetrisSpawn();
  if (gameActive) gameStatus = "Tetris: slide on Rank 1 = move block. Tap same square = rotate. Hold magnet off board = hard drop.";
  tetrisRender();
  gameRenderLEDs();
}

// ============================================================
// Snake Solo
// ============================================================
void snakePlaceFood() {
  uint8_t tries = 0;
  do {
    snakeFoodR = random(0, 8);
    snakeFoodC = random(0, 8);
    tries++;
    bool occupied = false;
    for (uint8_t i = 0; i < snakeLength; i++) if (snakeR[i] == snakeFoodR && snakeC[i] == snakeFoodC) { occupied = true; break; }
    if (!occupied && !gameIsH8(snakeFoodR, snakeFoodC)) return;
  } while (tries < 200);
  snakeFoodR = 6; snakeFoodC = 6;
}

void snakeRender() {
  gameClearGrid();
  for (uint8_t i = 0; i < snakeLength; i++) gameGridSet(snakeR[i], snakeC[i], i == snakeLength - 1 ? 1 : 7);
  gameGridSet(snakeFoodR, snakeFoodC, 3);
  gameTargetRow = snakeFoodR; gameTargetCol = snakeFoodC;
}

void startSnake(uint8_t r, uint8_t c) {
  snakeLength = 1;
  snakeR[0] = r; snakeC[0] = c;
  if (c > 0) { snakeLength = 2; snakeR[1] = r; snakeC[1] = c - 1; }
  snakeHeadIndex = snakeLength - 1;
  snakeLastDR = 0; snakeLastDC = 1;
  gameScore = 0; gameMoves = 0;
  gameP1Row=r; gameP1Col=c;
  randomSeed(micros());
  snakePlaceFood();
  gameMode = GAME_SNAKE;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Snake Solo: move one square at a time. Eat the gold food and avoid the tail.";
  snakeRender();
  gameRenderLEDs();
}

void snakeMoveTo(uint8_t nr, uint8_t nc) {
  if (!gameCardinalOneStep(snakeR[snakeLength - 1], snakeC[snakeLength - 1], nr, nc)) {
    gameStatus = "SNAKE: move exactly one square UP, DOWN, LEFT or RIGHT.";
    snakeRender(); gameRenderLEDs(); return;
  }
  for (uint8_t i = 0; i < snakeLength; i++) {
    if (snakeR[i] == nr && snakeC[i] == nc) {
      gameActive = false;
      gameStatus = "SNAKE GAME OVER - hit your tail.";
      snakeRender(); return;
    }
  }
  if (!gameValidSquare(nr, nc)) {
    gameActive = false;
    gameStatus = "SNAKE GAME OVER - hit the wall.";
    snakeRender(); return;
  }

  bool ate = gameSameSquare(nr, nc, snakeFoodR, snakeFoodC);
  if (ate && snakeLength < 64) {
    snakeR[snakeLength] = nr;
    snakeC[snakeLength] = nc;
    snakeLength++;
    gameScore++;
  } else {
    for (uint8_t i = 0; i + 1 < snakeLength; i++) {
      snakeR[i] = snakeR[i + 1];
      snakeC[i] = snakeC[i + 1];
    }
    snakeR[snakeLength - 1] = nr;
    snakeC[snakeLength - 1] = nc;
  }
  snakeHeadIndex = snakeLength - 1;
  gameMoves++;
  if (ate) {
    snakePlaceFood();
    gameStatus = "FOOD! Length: " + String(snakeLength) + " Score: " + String(gameScore);
  } else {
    gameStatus = "Good move. Keep steering the snake.";
  }
  snakeRender(); gameRenderLEDs();
}

// ============================================================
// Minesweeper
// ============================================================
uint8_t mineAdjacentCount(uint8_t r, uint8_t c) {
  uint8_t count = 0;
  for (int dr = -1; dr <= 1; dr++) for (int dc = -1; dc <= 1; dc++) {
    if (dr == 0 && dc == 0) continue;
    int nr = (int)r + dr, nc = (int)c + dc;
    if (gameValidSquare(nr, nc) && minePresent[nr * 8 + nc]) count++;
  }
  return count;
}

void mineReveal(uint8_t r, uint8_t c) {
  if (mineFlagged[r * 8 + c] || mineRevealed[r * 8 + c]) return;
  if (minePresent[r * 8 + c]) {
    mineRevealed[r * 8 + c] = true;
    gameActive = false;
    gameStatus = "BOOM - mine hit. Minesweeper game over.";
    return;
  }

  uint8_t queue[64];
  uint8_t head = 0, tail = 0;
  queue[tail++] = r * 8 + c;
  while (head < tail) {
    uint8_t idx = queue[head++];
    if (mineRevealed[idx] || mineFlagged[idx] || minePresent[idx]) continue;
    mineRevealed[idx] = true;
    mineRevealedSafe++;
    uint8_t rr = idx / 8, cc = idx % 8;
    if (mineAdjacentCount(rr, cc) == 0) {
      for (int dr = -1; dr <= 1; dr++) for (int dc = -1; dc <= 1; dc++) {
        int nr = (int)rr + dr, nc = (int)cc + dc;
        if (!gameValidSquare(nr, nc)) continue;
        uint8_t ni = nr * 8 + nc;
        if (!mineRevealed[ni] && !minePresent[ni] && !mineFlagged[ni] && tail < 64) queue[tail++] = ni;
      }
    }
  }
}

void minesGenerate() {
  memset(minePresent, 0, sizeof(minePresent));
  memset(mineRevealed, 0, sizeof(mineRevealed));
  memset(mineFlagged, 0, sizeof(mineFlagged));
  mineRevealedSafe = 0;
  uint8_t placed = 0;
  while (placed < mineCountTotal) {
    uint8_t r = random(0, 8), c = random(0, 8);
    if (gameIsH8(r, c)) continue;
    if (gameSameSquare(r, c, mineCursorR, mineCursorC)) continue;
    if (gameSameSquare(r, c, mineFlagR, mineFlagC)) continue;
    uint8_t idx = r * 8 + c;
    if (!minePresent[idx]) { minePresent[idx] = true; placed++; }
  }
  mineReveal(mineCursorR, mineCursorC);
  gameActive = true;
}

void minesRender() {
  gameClearGrid();
  for (uint8_t r = 0; r < 8; r++) for (uint8_t c = 0; c < 8; c++) {
    uint8_t idx = r * 8 + c;
    if (!mineRevealed[idx]) {
      if (mineFlagged[idx]) gameGrid[idx] = 9;
    } else {
      uint8_t n = mineAdjacentCount(r, c);
      gameGrid[idx] = 20 + n;
    }
    if (!gameActive && minePresent[idx]) gameGrid[idx] = 8;
  }
  if (gameValidSquare(mineCursorR, mineCursorC)) gameGridSet(mineCursorR, mineCursorC, 1);
  if (gameValidSquare(mineFlagR, mineFlagC) && !gameSameSquare(mineFlagR, mineFlagC, mineCursorR, mineCursorC)) gameGridSet(mineFlagR, mineFlagC, 2);
}

void minesRevealCursor() {
  mineReveal(mineCursorR, mineCursorC);
  if (gameActive) {
    uint16_t safeTotal = 63 - mineCountTotal;
    if (mineRevealedSafe >= safeTotal) {
      gameScore += 50;
      gameActive = false;
      gameStatus = "MINESWEEPER CLEARED! Score: " + String(gameScore);
    } else {
      gameScore++;
      gameStatus = "Square revealed. Tap the cursor again or move it one square.";
    }
  }
  minesRender();
  gameRenderLEDs();
}

void minesFlagAt(uint8_t r, uint8_t c) {
  uint8_t idx = r * 8 + c;
  if (!mineRevealed[idx]) {
    mineFlagged[idx] = !mineFlagged[idx];
    gameMoves++;
    gameStatus = mineFlagged[idx] ? "FLAG SET on " + squareName(r, c) : "FLAG REMOVED from " + squareName(r, c);
  } else {
    gameStatus = "That square is already revealed.";
  }
  minesRender(); gameRenderLEDs();
}

void startMinesweeper(uint8_t r1, uint8_t c1, uint8_t r2, uint8_t c2) {
  if (gameSameSquare(r1, c1, r2, c2)) {
    gameActive = false; gameHasBaseline = false;
    gameStatus = "MINESWEEPER START: cursor and flag piece must be on different squares.";
    clearLEDs(); return;
  }
  mineCursorR = r1; mineCursorC = c1;
  mineFlagR = r2; mineFlagC = c2;
  gameP1Row=r1; gameP1Col=c1; gameP2Row=r2; gameP2Col=c2;
  gameScore = 0; gameMoves = 0;
  randomSeed(micros());
  gameMode = GAME_MINESWEEPER;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  minesGenerate();
  gameStatus = "Minesweeper: P1 cursor. Tap cursor = reveal. P2 flag piece placed on a square toggles a flag.";
  minesRender(); gameRenderLEDs();
}

// ============================================================
// Whack-a-Mole
// ============================================================
void whackNewMole() {
  uint8_t oldR = whackMoleR, oldC = whackMoleC;
  uint8_t tries = 0;
  do {
    whackMoleR = random(0, 8); whackMoleC = random(0, 8); tries++;
  } while ((gameIsH8(whackMoleR, whackMoleC) || gameSameSquare(whackMoleR, whackMoleC, oldR, oldC) || gameSameSquare(whackMoleR, whackMoleC, whackPlayerR, whackPlayerC)) && tries < 100);
}

void whackRender() {
  gameClearGrid();
  gameGridSet(whackMoleR, whackMoleC, 3);
  gameGridSet(whackPlayerR, whackPlayerC, 1);
  gameTargetRow = whackMoleR; gameTargetCol = whackMoleC;
}

void whackHitCheck() {
  if (gameSameSquare(whackPlayerR, whackPlayerC, whackMoleR, whackMoleC)) {
    whackHits++;
    gameScore++;
    gameMoves++;
    whackNewMole();
    whackMoleTimer = millis();
    gameStatus = "WHACK! Score: " + String(gameScore);
  }
}

void whackTick() {
  unsigned long now = millis();
  if (now - whackStartTime >= WHACK_GAME_MS) {
    gameActive = false;
    gameStatus = "TIME UP! Whacks: " + String(gameScore) + ". Press START again.";
    whackRender();
    return;
  }
  if (now - whackMoleTimer >= WHACK_MOLE_MS) {
    whackMoleTimer = now;
    whackNewMole();
    gameStatus = "NEW MOLE! Reach the lit square fast.";
  }
  whackRender();
  gameRenderLEDs();
}

void startWhack(uint8_t r, uint8_t c) {
  whackPlayerR = r; whackPlayerC = c;
  gameP1Row=r; gameP1Col=c;
  whackHits = 0;
  gameScore = 0; gameMoves = 0;
  whackStartTime = millis();
  whackMoleTimer = millis();
  randomSeed(micros());
  whackNewMole();
  gameMode = GAME_WHACK;
  gameActive = true;
  gameHasBaseline = true;
  gameWaiting = false;
  animationMode = ANIM_NONE;
  gameStatus = "Whack-a-Mole: move to the lit mole and tap by lifting + returning on the same square.";
  whackRender(); gameRenderLEDs();
}


// ============================================================
// V11 ADVANCED GAMES
// ============================================================
void lightsOutToggle(uint8_t r,uint8_t c){
  const int dirs[5][2]={{0,0},{1,0},{-1,0},{0,1},{0,-1}};
  for(uint8_t k=0;k<5;k++){int rr=(int)r+dirs[k][0],cc=(int)c+dirs[k][1];if(gameValidSquare(rr,cc))lightsOutOn[rr*8+cc]=!lightsOutOn[rr*8+cc];}
}
bool lightsOutSolved(){for(uint8_t i=0;i<64;i++)if(lightsOutOn[i])return false;return true;}
void lightsOutRender(){
  gameClearGrid();
  for(uint8_t r=0;r<8;r++)for(uint8_t c=0;c<8;c++)if(lightsOutOn[r*8+c])gameGridSet(r,c,6);
  gameGridSet(lightsOutPlayerR,lightsOutPlayerC,1);
}
void startLightsOut(uint8_t r,uint8_t c){
  memset(lightsOutOn,0,sizeof(lightsOutOn));lightsOutPlayerR=r;lightsOutPlayerC=c;lightsOutMoves=0;gameScore=0;gameMoves=0;
  randomSeed(micros());
  for(uint8_t k=0;k<18;k++)lightsOutToggle(random(0,8),random(0,8));
  gameMode=GAME_LIGHTS_OUT;gameActive=true;gameHasBaseline=true;gameWaiting=false;animationMode=ANIM_NONE;gameStatus="Lights Out: tap a square to toggle it and its four neighbors. Turn every light off.";lightsOutRender();gameRenderLEDs();
}

void mazeBuild(){
  memset(mazeWalls,0,sizeof(mazeWalls));
  const uint8_t walls[][2]={{0,2},{1,2},{2,0},{2,1},{2,2},{2,4},{2,5},{2,6},{3,6},{4,1},{4,2},{4,3},{4,5},{5,5},{6,1},{6,3},{6,4},{6,5},{7,5}};
  for(size_t i=0;i<sizeof(walls)/sizeof(walls[0]);i++)mazeWalls[walls[i][0]*8+walls[i][1]]=true;
  mazeWalls[0]=false;mazeWalls[mazeExitR*8+mazeExitC]=false;
}
void mazeRender(){
  gameClearGrid();
  for(uint8_t r=0;r<8;r++)for(uint8_t c=0;c<8;c++)if(mazeWalls[r*8+c])gameGridSet(r,c,6);
  gameGridSet(mazeExitR,mazeExitC,3);gameGridSet(mazePlayerR,mazePlayerC,1);gameTargetRow=mazeExitR;gameTargetCol=mazeExitC;
}
void startMazeRunner(uint8_t r,uint8_t c){
  mazeBuild();
  // If the starting piece is inside a wall, find the nearest open cell.
  if(mazeWalls[r*8+c]){r=0;c=0;}
  mazePlayerR=r;mazePlayerC=c;gameP1Row=r;gameP1Col=c;gameScore=0;gameMoves=0;gameLives=1;gameMode=GAME_MAZE_RUNNER;gameActive=true;gameHasBaseline=true;gameWaiting=false;animationMode=ANIM_NONE;gameStatus="Maze Runner: reach H8. Move one square at a time; purple walls are blocked.";mazeRender();gameRenderLEDs();
}

void memoryBuildSequence(){uint8_t start=(uint8_t)(gameP1Row*8+gameP1Col);for(uint8_t i=0;i<memoryLength;i++){uint8_t v;do{v=(uint8_t)random(0,64);}while(v==start);memorySequence[i]=v;}}
void memoryBeginPlayback(){memoryPlaying=true;memoryPlayIndex=0;memoryStep=0;memoryTimer=millis();gameStatus="MEMORY: watch the yellow sequence.";gameRenderLEDs();}
void memoryRender(){
  gameClearGrid();
  // Player square stays visible; during playback the current sequence cell is yellow.
  gameGridSet(gameP1Row,gameP1Col,1);
  if(memoryPlaying){if(memoryPlayIndex<memoryLength){uint8_t idx=memorySequence[memoryPlayIndex];gameGridSet(idx/8,idx%8,12);}}
  else if(memoryStep<memoryLength){uint8_t idx=memorySequence[memoryStep];gameGridSet(idx/8,idx%8,3);gameTargetRow=idx/8;gameTargetCol=idx%8;}
}
void startMemoryPath(uint8_t r,uint8_t c){
  memoryLength=4;memoryLives=3;memoryStep=0;memoryPlayIndex=0;memoryPlaying=false;gameP1Row=r;gameP1Col=c;gameLives=memoryLives;gameScore=0;gameMoves=0;randomSeed(micros());memoryBuildSequence();gameMode=GAME_MEMORY_PATH;gameActive=true;gameHasBaseline=true;gameWaiting=false;animationMode=ANIM_NONE;memoryBeginPlayback();memoryRender();gameRenderLEDs();
}
void memoryTick(){
  if(!gameActive)return;unsigned long now=millis();if(now-memoryTimer<MEMORY_FLASH_MS)return;memoryTimer=now;
  if(memoryPlaying){memoryPlayIndex++;if(memoryPlayIndex>=memoryLength){memoryPlaying=false;memoryStep=0;gameStatus="MEMORY: replay the yellow sequence in order. Use adjacent moves and return to each target.";}memoryRender();gameRenderLEDs();return;}
  memoryRender();gameRenderLEDs();
}

// ============================================================
// Board-change input handling
// ============================================================
void gameBeginSingleLift(uint8_t oldR, uint8_t oldC) {
  gameRejectReturn = false;
  gameWaiting = true;
  gameMovingPlayer = 1;
  gameWaitRow = oldR;
  gameWaitCol = oldC;
  gameWaitSince = millis();
}

void gameBeginTwoLift(uint8_t player, uint8_t oldR, uint8_t oldC) {
  gameRejectReturn = false;
  gameWaiting = true;
  gameMovingPlayer = player;
  gameWaitRow = oldR;
  gameWaitCol = oldC;
  gameWaitSince = millis();
}

void gameRejectInvalidDestination(uint8_t nr, uint8_t nc, const String &reason) {
  gameWaiting = false;
  gameRejectReturn = true;
  gameRejectRow = nr;
  gameRejectCol = nc;
  gameReturnRow = gameWaitRow;
  gameReturnCol = gameWaitCol;
  gameStatus = "INVALID MOVE: " + reason + " RED = illegal square. Return to " + squareName(gameReturnRow, gameReturnCol) + ".";
}

void gameClearInvalidReturn() {
  gameRejectReturn = false;
  gameWaiting = false;
  gameStatus = "Returned to " + squareName(gameWaitRow, gameWaitCol) + ". Blue squares are legal moves.";
}

bool gameWasTap(uint8_t r, uint8_t c) {
  return gameSameSquare(r, c, gameWaitRow, gameWaitCol) && (millis() - gameWaitSince <= GAME_TAP_WINDOW_MS);
}

void gameRejectBadOneStep(const String &what) {
  gameStatus = what;
  gameMoves++;
}

void gameProcessSinglePlacement(uint8_t nr, uint8_t nc) {
  bool tap = gameWasTap(nr, nc);
  uint8_t oldR = gameWaitRow, oldC = gameWaitCol;

  if (gameRejectReturn) {
    if (gameSameSquare(nr, nc, gameReturnRow, gameReturnCol)) gameClearInvalidReturn();
    else gameStatus = "WARNING: return the piece to " + squareName(gameReturnRow, gameReturnCol) + " first. RED is blocked.";
    gameRenderLEDs();
    return;
  }

  // A tap is only special on the same square. A physical jump to another
  // square must obey that game's movement rules before it is accepted.
  if (!tap && !gameCanMoveTo(1, oldR, oldC, nr, nc)) {
    gameRejectInvalidDestination(nr, nc, "move only to the BLUE squares shown on the board");
    gameRenderLEDs();
    return;
  }

  gameWaiting = false;

  switch (gameMode) {
    case GAME_BREAKOUT:
      breakoutPaddleC = nc; gameP1Row=0; gameP1Col=nc; gameMoves++;
      break;

    case GAME_SPACE_INVADERS:
      siShipC = nc; gameP1Row=0; gameP1Col=nc; gameMoves++;
      if (tap) spaceFire();
      break;

    case GAME_FROGGER:
      frogR = nr; frogC = nc; gameP1Row=frogR; gameP1Col=frogC; gameMoves++;
      if (frogHit()) {
        if (gameLives > 0) gameLives--;
        if (gameLives == 0) { gameActive = false; gameStatus = "FROGGER GAME OVER."; }
        else { frogR = 0; frogC = 3; gameStatus = "HIT! Lives left: " + String(gameLives); }
      } else if (frogR == 7) {
        gameScore++; frogR = 0; frogC = 3; gameStatus = "SAFE! Frogger score: " + String(gameScore);
      }
      break;

    case GAME_FLAPPY:
      flappyControlR = nr; flappyControlC = nc; gameP1Row=nr; gameP1Col=nc; flappyBirdC = nc; gameMoves++;
      if (tap) flappyTap();
      break;

    case GAME_TETRIS:
      tetrisLiftWaiting = false;
      tetrisMoveController(nc); gameP1Row=0; gameP1Col=nc;
      if (tap) tetrisRotate();
      break;

    case GAME_SNAKE:
      snakeMoveTo(nr, nc);
      if (gameActive) { gameP1Row=snakeR[snakeLength-1]; gameP1Col=snakeC[snakeLength-1]; }
      break;

    case GAME_WHACK:
      whackPlayerR = nr; whackPlayerC = nc; gameP1Row=nr; gameP1Col=nc; gameMoves++;
      whackHitCheck();
      break;

    case GAME_LIGHTS_OUT:
      lightsOutPlayerR=nr; lightsOutPlayerC=nc; gameP1Row=nr; gameP1Col=nc; gameMoves++;
      if(tap){lightsOutToggle(nr,nc);gameScore=++lightsOutMoves;if(lightsOutSolved()){gameActive=false;gameStatus="LIGHTS OUT CLEARED! Score: "+String(gameScore);}}
      break;

    case GAME_MAZE_RUNNER:
      mazePlayerR=nr; mazePlayerC=nc; gameP1Row=nr; gameP1Col=nc; gameMoves++;
      if(nr==mazeExitR&&nc==mazeExitC){gameScore=gameMoves;gameActive=false;gameStatus="MAZE CLEARED! Moves: "+String(gameMoves);}
      break;

    case GAME_MEMORY_PATH:
      gameP1Row=nr; gameP1Col=nc; gameMoves++;
      if(tap && !memoryPlaying){
        uint8_t idx=nr*8+nc;
        if(idx==memorySequence[memoryStep]){
          memoryStep++;gameScore++;
          if(memoryStep>=memoryLength){if(memoryLength<24)memoryLength++;gameStatus="Sequence complete! Next round is longer.";memoryBuildSequence();memoryBeginPlayback();}
          else gameStatus="Correct. Continue to the next yellow square.";
        }else{
          if(memoryLives>0)memoryLives--;gameLives=memoryLives;memoryStep=0;
          if(memoryLives==0){gameActive=false;gameStatus="MEMORY GAME OVER. Press START again.";}
          else {gameStatus="Wrong square. Lives left: "+String(memoryLives)+". Watch the sequence again.";memoryBeginPlayback();}
        }
      }
      break;

    default:
      break;
  }

  if (gameMode == GAME_BREAKOUT) gameStatus = "Paddle column: " + String((char)('A' + breakoutPaddleC));
  if (gameMode == GAME_SPACE_INVADERS && !tap) gameStatus = "Ship column: " + String((char)('A' + siShipC));

  if (gameActive) {
    switch (gameMode) {
      case GAME_BREAKOUT: breakoutRender(); break;
      case GAME_SPACE_INVADERS: spaceRender(); break;
      case GAME_FROGGER: frogRender(); break;
      case GAME_FLAPPY: flappyRender(); break;
      case GAME_TETRIS: tetrisRender(); break;
      case GAME_SNAKE: snakeRender(); break;
      case GAME_WHACK: whackRender(); break;
      case GAME_LIGHTS_OUT: lightsOutRender(); break;
      case GAME_MAZE_RUNNER: mazeRender(); break;
      case GAME_MEMORY_PATH: memoryRender(); break;
      default: break;
    }
    gameRenderLEDs();
  }
}

void gameProcessTwoPlacement(uint8_t nr, uint8_t nc) {
  uint8_t moving = gameMovingPlayer;
  bool tap = gameWasTap(nr, nc);
  uint8_t oldR = gameWaitRow, oldC = gameWaitCol;

  if (gameRejectReturn) {
    if (gameSameSquare(nr, nc, gameReturnRow, gameReturnCol)) gameClearInvalidReturn();
    else gameStatus = "WARNING: return the piece to " + squareName(gameReturnRow, gameReturnCol) + " first. RED is blocked.";
    gameRenderLEDs();
    return;
  }

  if (moving == 1 && gameSameSquare(nr, nc, gameP2Row, gameP2Col)) {
    gameRejectInvalidDestination(nr,nc,"you cannot occupy Player 2's square");
    gameRenderLEDs(); return;
  }
  if (moving == 2 && gameSameSquare(nr, nc, gameP1Row, gameP1Col)) {
    gameRejectInvalidDestination(nr,nc,"you cannot occupy Player 1's square");
    gameRenderLEDs(); return;
  }

  // Movement games are strict: only the blue adjacent squares can be chosen.
  // Tap is allowed only where the game defines a tap action.
  if (gameMode == GAME_TRON || gameMode == GAME_TANKS || gameMode == GAME_PONG || gameMode == GAME_MINESWEEPER) {
    if (!tap && !gameCanMoveTo(moving, oldR, oldC, nr, nc)) {
      gameRejectInvalidDestination(nr, nc, "move exactly ONE legal square at a time");
      gameRenderLEDs(); return;
    }
  }

  gameWaiting = false;

  if (gameMode == GAME_PONG) {
    if (moving == 1) { pongP1R = nr; gameP1Row=nr; gameP1Col=0; }
    else { pongP2R = nr; gameP2Row=nr; gameP2Col=7; }
    gameMoves++;
    pongRender(); gameRenderLEDs(); return;
  }

  if (gameMode == GAME_TRON) {
    if (tap) { gameStatus = "TRON: use a new blue square for your next move."; gameRenderLEDs(); return; }
    tronApplyMove(moving, nr, nc);
    gameP1Row=tronP1R; gameP1Col=tronP1C; gameP2Row=tronP2R; gameP2Col=tronP2C;
    return;
  }

  if (gameMode == GAME_TANKS) {
    if (tap) tankFire(moving);
    else {
      if (moving == 1) { tankP1R = nr; tankP1C = nc; gameP1Row=nr; gameP1Col=nc; }
      else { tankP2R = nr; tankP2C = nc; gameP2Row=nr; gameP2Col=nc; }
      gameMoves++;
    }
    tankRender(); gameRenderLEDs(); return;
  }

  if (gameMode == GAME_MINESWEEPER) {
    if (moving == 1) {
      if (tap) minesRevealCursor();
      else { mineCursorR = nr; mineCursorC = nc; gameP1Row=nr; gameP1Col=nc; gameMoves++; gameStatus = "Cursor moved. Blue squares are the next legal moves. Tap to reveal."; }
      minesRender(); gameRenderLEDs();
    } else {
      // Flag piece is also one-step at a time.
      if (tap) gameStatus = "FLAG: move the flag piece one blue square at a time.";
      else { mineFlagR = nr; mineFlagC = nc; gameP2Row=nr; gameP2Col=nc; minesFlagAt(nr,nc); }
      minesRender(); gameRenderLEDs();
    }
    return;
  }
}

void gameGetTwoPiecePosition(uint8_t player, uint8_t &r, uint8_t &c) {
  if (gameMode == GAME_PONG) {
    if (player == 1) { r = pongP1R; c = 0; }
    else { r = pongP2R; c = 7; }
    return;
  }
  if (gameMode == GAME_TRON) {
    if (player == 1) { r = tronP1R; c = tronP1C; }
    else { r = tronP2R; c = tronP2C; }
    return;
  }
  if (gameMode == GAME_TANKS) {
    if (player == 1) { r = tankP1R; c = tankP1C; }
    else { r = tankP2R; c = tankP2C; }
    return;
  }
  if (gameMode == GAME_MINESWEEPER) {
    if (player == 1) { r = mineCursorR; c = mineCursorC; }
    else { r = mineFlagR; c = mineFlagC; }
    return;
  }
  r = player == 1 ? gameP1Row : gameP2Row;
  c = player == 1 ? gameP1Col : gameP2Col;
}

void gameProcessBoardChange() {
  if (!gameActive || !gameHasBaseline) return;

  uint16_t active = activeSwitchCount();

  if (gameMode == GAME_PONG || gameMode == GAME_TRON || gameMode == GAME_TANKS || gameMode == GAME_MINESWEEPER) {
    uint8_t p1r=0,p1c=0,p2r=0,p2c=0;
    gameGetTwoPiecePosition(1,p1r,p1c);
    gameGetTwoPiecePosition(2,p2r,p2c);

    if (gameRejectReturn) {
      if (active == 1) {
        uint8_t rr,cc;
        if (gameFindSingle(rr,cc) && gameSameSquare(rr,cc,gameReturnRow,gameReturnCol)) gameClearInvalidReturn();
        else gameStatus = "WARNING: RED = illegal square. Return to " + squareName(gameReturnRow, gameReturnCol) + ".";
      }
      gameRenderLEDs();
      return;
    }

    if (!gameWaiting && active == 1) {
      if (!matrixState[p1r][p1c]) gameBeginTwoLift(1,p1r,p1c);
      else if (!matrixState[p2r][p2c]) gameBeginTwoLift(2,p2r,p2c);
      else gameStatus = "Two-piece game: keep exactly TWO magnets on the board.";
      gameRenderLEDs();
      return;
    }
    if (gameWaiting && active == 2) {
      uint8_t nr=0,nc=0;
      bool foundNew = false;
      for (uint8_t r = 0; r < 8 && !foundNew; r++) {
        for (uint8_t c = 0; c < 8; c++) {
          if (!matrixState[r][c]) continue;
          if (gameMovingPlayer == 1 && gameSameSquare(r,c,p2r,p2c)) continue;
          if (gameMovingPlayer == 2 && gameSameSquare(r,c,p1r,p1c)) continue;
          nr=r; nc=c; foundNew=true; break;
        }
      }
      if (foundNew) gameProcessTwoPlacement(nr,nc);
      return;
    }
    if (gameWaiting && active == 1) {
      // Keep waiting; a tap returns to the same square, and a valid move requires 2 sensors.
      gameRenderLEDs();
      return;
    }
    return;
  }

  // Single-piece games.
  if (gameMode == GAME_MEMORY_PATH && memoryPlaying) { gameRenderLEDs(); return; }
  if (gameRejectReturn) {
    if (active == 1) {
      uint8_t nr=0,nc=0;
      if (gameFindSingle(nr,nc) && gameSameSquare(nr,nc,gameReturnRow,gameReturnCol)) gameClearInvalidReturn();
      else gameStatus = "WARNING: RED = illegal square. Return to " + squareName(gameReturnRow, gameReturnCol) + ".";
    }
    gameRenderLEDs();
    return;
  }

  if (!gameWaiting && active == 0) {
    uint8_t oldR=0, oldC=0;
    switch (gameMode) {
      case GAME_BREAKOUT: oldR = 0; oldC = breakoutPaddleC; break;
      case GAME_SPACE_INVADERS: oldR = 0; oldC = siShipC; break;
      case GAME_FROGGER: oldR = frogR; oldC = frogC; break;
      case GAME_FLAPPY: oldR = flappyControlR; oldC = flappyControlC; break;
      case GAME_TETRIS: oldR = 0; oldC = tetrisControlC; tetrisLiftWaiting = true; tetrisLiftSince = millis(); break;
      case GAME_SNAKE: oldR = snakeR[snakeLength - 1]; oldC = snakeC[snakeLength - 1]; break;
      case GAME_WHACK: oldR = whackPlayerR; oldC = whackPlayerC; break;
      case GAME_LIGHTS_OUT: oldR = lightsOutPlayerR; oldC = lightsOutPlayerC; break;
      case GAME_MAZE_RUNNER: oldR = mazePlayerR; oldC = mazePlayerC; break;
      case GAME_MEMORY_PATH: oldR = gameP1Row; oldC = gameP1Col; break;
      default: break;
    }
    gameBeginSingleLift(oldR, oldC);
    gameStatus = "MAGNET LIFTED. BLUE squares = legal moves. Same square quickly = TAP.";
    gameRenderLEDs();
    return;
  }

  if (gameWaiting && active == 1) {
    uint8_t nr=0,nc=0;
    if (gameFindSingle(nr,nc)) gameProcessSinglePlacement(nr,nc);
    return;
  }
}

// ============================================================
// Game starts
// ============================================================
void gameStartSelected(const String &type) {
  gameStop();
  uint8_t r1,c1,r2,c2;
  randomSeed(micros());

  if (type == "breakout") {
    if (gameRequireSingle(true,r1,c1)) startBreakout(r1,c1);
  } else if (type == "invaders") {
    if (gameRequireSingle(true,r1,c1)) startSpaceInvaders(r1,c1);
  } else if (type == "frogger") {
    if (gameRequireSingle(true,r1,c1)) startFrogger(r1,c1);
  } else if (type == "flappy") {
    if (gameRequireSingle(false,r1,c1)) startFlappy(r1,c1);
  } else if (type == "pong") {
    if (gameRequireTwo(r1,c1,r2,c2)) startPong(r1,c1,r2,c2);
  } else if (type == "tron") {
    if (gameRequireTwo(r1,c1,r2,c2)) startTron(r1,c1,r2,c2);
  } else if (type == "tanks") {
    if (gameRequireTwo(r1,c1,r2,c2)) startTanks(r1,c1,r2,c2);
  } else if (type == "tetris") {
    if (gameRequireSingle(true,r1,c1)) startTetris(r1,c1);
  } else if (type == "snake") {
    if (gameRequireSingle(false,r1,c1)) startSnake(r1,c1);
  } else if (type == "minesweeper") {
    if (gameRequireTwo(r1,c1,r2,c2)) startMinesweeper(r1,c1,r2,c2);
  } else if (type == "whack") {
    if (gameRequireSingle(false,r1,c1)) startWhack(r1,c1);
  } else if (type == "lightsout") {
    if (gameRequireSingle(false,r1,c1)) startLightsOut(r1,c1);
  } else if (type == "maze") {
    if (gameRequireSingle(false,r1,c1)) startMazeRunner(r1,c1);
  } else if (type == "memory") {
    if (gameRequireSingle(false,r1,c1)) startMemoryPath(r1,c1);
  } else {
    gameStatus = "Unknown game.";
    clearLEDs();
  }
}

void gameStart() {
  gameStartSelected("breakout");
}

void gameUpdate() {
  if (!gameActive) return;
  switch (gameMode) {
    case GAME_BREAKOUT: breakoutTick(); break;
    case GAME_SPACE_INVADERS: spaceTick(); break;
    case GAME_FROGGER: frogTick(); break;
    case GAME_FLAPPY: flappyTick(); break;
    case GAME_PONG: pongTick(); break;
    case GAME_TANKS: tankTick(); break;
    case GAME_TETRIS:
      if (tetrisLiftWaiting && activeSwitchCount() == 0 && millis() - tetrisLiftSince >= TETRIS_HARD_DROP_MS) tetrisHardDrop();
      tetrisTick();
      break;
    case GAME_WHACK: whackTick(); break;
    case GAME_MEMORY_PATH: memoryTick(); break;
    default: break;
  }
}

// ============================================================
// Draw matrix LEDs
// ============================================================

void drawMatrixStatus() {
  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      uint8_t ledIndex = chessToLED(r, c);

      if (matrixState[r][c]) {
        leds[ledIndex] = CRGB(0, 255, 80);
      } else {
        leds[ledIndex] = CRGB::Black;
      }
    }
  }

  FastLED.show();
}

// ============================================================
// LED animations
// ============================================================

CRGB animColor(uint8_t palette, uint8_t index) {
  if (palette >= BUILTIN_ANIMATION_COUNT || index >= MAX_ANIMATION_COLORS) return CRGB::Black;
  return animationPalettes[palette][index];
}

CRGB rainbowPaletteColor(uint8_t phase) {
  const uint8_t p = 0;
  const uint8_t count = animationPaletteCounts[p];
  if (count == 0) return CRGB::Black;

  // Map 0..255 smoothly across all six user-editable rainbow colors.
  // The last color blends back into the first, so the cycle is seamless.
  uint16_t scaled = (uint16_t)phase * count;
  uint8_t segment = (uint8_t)(scaled >> 8);
  uint8_t fraction = (uint8_t)(scaled & 0xFF);
  if (segment >= count) segment = count - 1;
  uint8_t next = (segment + 1) % count;
  return blend(animColor(p, segment), animColor(p, next), fraction);
}

void animationRainbow() {
  // Smooth 2D rainbow across the chessboard. This uses the saved six-color
  // palette, but blends between colors instead of snapping from one color to
  // the next. The phase moves every frame, so the whole board visibly flows.
  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint16_t phase = (uint16_t)animationHue + (uint16_t)c * 32 + (uint16_t)r * 8;
      leds[chessToLED(r, c)] = rainbowPaletteColor((uint8_t)phase);
    }
  }

  // Faster, smoother motion than the old 2-step hue jump.
  animationHue += 5;
  FastLED.show();
}

void animationWave() {
  const uint8_t p = 1;
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t wave = sin8(animationHue + (i * 7));
    leds[i] = blend(animColor(p,0), animColor(p,1), wave);
  }
  animationHue += 3;
  FastLED.show();
}

void animationChase() {
  const uint8_t p = 2;
  fill_solid(leds, NUM_LEDS, animColor(p,0));
  uint8_t position = (millis() / max((int)animationSpeed, 1)) % NUM_LEDS;
  for (uint8_t x = 0; x < 5; x++) {
    int index = position - x;
    while (index < 0) index += NUM_LEDS;
    uint8_t colorIndex = 1 + (x % 2);
    leds[index] = animColor(p, colorIndex);
  }
  FastLED.show();
}

void animationScan() {
  const uint8_t p = 3;
  fill_solid(leds, NUM_LEDS, animColor(p,0));
  uint8_t position = (millis() / max((int)animationSpeed, 1)) % NUM_LEDS;
  leds[position] = animColor(p,1);
  if (position > 0) leds[position - 1] = animColor(p,2);
  if (position + 1 < NUM_LEDS) leds[position + 1] = animColor(p,2);
  FastLED.show();
}

void animationBreath() {
  const uint8_t p = 4;
  uint8_t value = beatsin8(max((int)animationSpeed / 10, 1), 20, 255);
  CRGB color = animColor(p,0);
  color.nscale8(value);
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

void animationRandom() {
  const uint8_t p = 5;
  const uint8_t count = animationPaletteCounts[p];
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    if (random(0, 5) == 0) leds[i] = animColor(p, random(0, count));
    else leds[i].fadeToBlackBy(35);
  }
  FastLED.show();
}

void animationCenterOut() {
  const uint8_t p = 6;
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  const uint8_t cx1 = 3, cx2 = 4, cy1 = 3, cy2 = 4;
  uint8_t radius = (millis() / max((int)animationSpeed, 1)) % 8;
  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t distance = 0;
      if (c < cx1) distance = max(distance, (uint8_t)(cx1 - c));
      else if (c > cx2) distance = max(distance, (uint8_t)(c - cx2));
      if (r < cy1) distance = max(distance, (uint8_t)(cy1 - r));
      else if (r > cy2) distance = max(distance, (uint8_t)(r - cy2));
      if (distance <= radius) {
        CRGB color = (distance % 2 == 0) ? animColor(p,0) : animColor(p,1);
        uint8_t level = 255 - min((int)distance * 32, 224);
        color.nscale8(level);
        leds[chessToLED(r,c)] = color;
      }
    }
  }
  uint8_t pulse = beatsin8(6, 120, 255);
  for (uint8_t r = 3; r <= 4; r++) for (uint8_t c = 3; c <= 4; c++) {
    leds[chessToLED(r,c)] = animColor(p,1);
    leds[chessToLED(r,c)].nscale8(pulse);
  }
  FastLED.show();
}

void animationCustom() {
  if (customAnimationFrameCount < 1) return;
  if(customAnimationStartedAt==0) customAnimationStartedAt=millis();
  if(customAnimationRepeatSeconds>0 && millis()-customAnimationStartedAt >= (unsigned long)customAnimationRepeatSeconds*1000UL){
    animationMode=ANIM_NONE;customAnimationStartedAt=0;clearLEDs();if(pieceTrackingEnabled)updatePieceTrackingLEDs();return;
  }
  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t idx = customAnimationFrames[customAnimationFrame][r*8+c];
      if (idx == 0 || idx > customAnimationColorCount) leds[chessToLED(r,c)] = CRGB::Black;
      else leds[chessToLED(r,c)] = customAnimationColors[idx - 1];
    }
  }
  customAnimationFrame = (customAnimationFrame + 1) % customAnimationFrameCount;
  FastLED.show();
}

// One-time startup animation. Runs from center -> outside using Center Out palette.
void runStartupAnimation() {
  animationMode = ANIM_CENTER_OUT;
  const uint16_t stepDelay = 90;
  for (uint8_t radius = 0; radius < 8; radius++) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for (uint8_t r = 0; r < 8; r++) {
      for (uint8_t c = 0; c < 8; c++) {
        uint8_t distance = 0;
        if (c < 3) distance = max(distance, (uint8_t)(3 - c));
        else if (c > 4) distance = max(distance, (uint8_t)(c - 4));
        if (r < 3) distance = max(distance, (uint8_t)(3 - r));
        else if (r > 4) distance = max(distance, (uint8_t)(r - 4));
        if (distance <= radius) {
          CRGB color = (distance % 2 == 0) ? animColor(6,0) : animColor(6,1);
          color.nscale8(255 - min((int)distance * 32, 224));
          leds[chessToLED(r,c)] = color;
        }
      }
    }
    FastLED.show();
    delay(stepDelay);
  }
  for (uint8_t fade = 0; fade < 8; fade++) { fadeToBlackBy(leds, NUM_LEDS, 32); FastLED.show(); delay(35); }
  clearLEDs();
  animationMode = ANIM_NONE;
}

void updateAnimation() {
  if (gameActive) return;
  if (animationMode == ANIM_NONE) return;
  unsigned long now = millis();
  unsigned long interval = (animationMode == ANIM_CUSTOM) ? customAnimationFrameDelay : max(10, (int)animationSpeed / 2);
  if (now - lastAnimationUpdate < interval) return;
  lastAnimationUpdate = now;
  switch (animationMode) {
    case ANIM_RAINBOW: animationRainbow(); break;
    case ANIM_WAVE: animationWave(); break;
    case ANIM_CHASE: animationChase(); break;
    case ANIM_SCAN: animationScan(); break;
    case ANIM_BREATH: animationBreath(); break;
    case ANIM_RANDOM: animationRandom(); break;
    case ANIM_CENTER_OUT: animationCenterOut(); break;
    case ANIM_CUSTOM: animationCustom(); break;
    default: break;
  }
}

// ============================================================
// Web page
// ============================================================

const char INDEX_HTML[] PROGMEM = R"rawliteral(

<!doctype html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1"><title>Smart Chess V12 // Hardware Console</title>
<style>
*{box-sizing:border-box}body{margin:0;background:#07101d;color:#eef4ff;font-family:Arial,sans-serif}header{position:sticky;top:0;z-index:9;background:#091321f5;border-bottom:1px solid #243653;padding:15px}.wrap{max-width:1500px;margin:auto;padding:12px}.sub{color:#9aacC8;font-size:12px;margin-top:4px}.stats{display:flex;flex-wrap:wrap;gap:7px;margin-bottom:12px}.badge{background:#0e1929;border:1px solid #263752;border-radius:10px;padding:8px 10px;font-size:12px}.good{color:#46ee99}.bad{color:#ff7078}.grid{display:grid;grid-template-columns:minmax(320px,1fr) minmax(340px,1.1fr);gap:12px}.card{background:#101a2a;border:1px solid #263650;border-radius:15px;padding:14px;margin-bottom:12px}.note{color:#9badc7;font-size:12px;line-height:1.45}.board{display:grid;grid-template-columns:repeat(8,1fr);width:min(94vw,690px);aspect-ratio:1;margin:auto;border:3px solid #4f6380;border-radius:9px;overflow:hidden}.sq{position:relative;display:flex;align-items:center;justify-content:center;font-weight:700;font-size:12px;user-select:none;touch-action:manipulation}.sq:nth-child(odd){background:#d9dde3;color:#111820}.sq:nth-child(even){background:#6b788e;color:white}.sq.on{box-shadow:inset 0 0 0 3px #35f29b,inset 0 0 26px rgba(0,255,145,.45)}.sq.target{box-shadow:inset 0 0 0 4px #ffd230,inset 0 0 28px rgba(255,210,48,.55)}.sq.guide{box-shadow:inset 0 0 0 4px #42aaff,inset 0 0 20px rgba(40,165,255,.45)}.sq.p1{box-shadow:inset 0 0 0 4px #19dcff,inset 0 0 26px rgba(25,220,255,.5)}.sq.p2{box-shadow:inset 0 0 0 4px #ff5bb5,inset 0 0 26px rgba(255,90,180,.5)}.sq.disabled{opacity:.33}.coord{position:absolute;right:3px;bottom:2px;font-size:9px;opacity:.6}.row{display:flex;flex-wrap:wrap;gap:6px;align-items:center}button,select,input{font:inherit}button{background:#19273d;color:white;border:1px solid #394d6d;border-radius:9px;padding:9px 11px;min-height:38px;cursor:pointer;touch-action:manipulation}button:hover{background:#253957}.primary{background:#2861b9;border-color:#4b8bef}.danger{background:#6b3038;border-color:#9b4b55}select{background:#0c1524;color:white;border:1px solid #394d69;border-radius:8px;padding:8px;width:100%}input[type=range]{width:100%}input[type=color]{width:68px;height:40px;background:transparent;border:0}.event{background:#080e17;border:1px solid #223149;border-radius:9px;padding:10px;font-family:monospace;white-space:pre-wrap;overflow:auto;min-height:50px}.map{display:grid;grid-template-columns:1fr 1fr;gap:7px}.mapitem{display:flex;gap:7px;align-items:center;background:#0c1524;border-radius:9px;padding:7px}.mapitem b{width:55px;font-size:12px}.gamegrid{display:grid;grid-template-columns:repeat(3,1fr);gap:8px}.game{background:#0c1524;border:1px solid #2a3952;border-radius:11px;padding:10px}.game h3{font-size:14px;margin:0 0 6px}.game p{font-size:11px;line-height:1.45;color:#9badc7;min-height:90px;margin:0 0 8px}.gameMeta{font-size:10px;color:#c5d0df;min-height:28px;margin-bottom:8px}.sq.g1{box-shadow:inset 0 0 0 4px #19dcff,inset 0 0 24px rgba(25,220,255,.55)}.sq.g2{box-shadow:inset 0 0 0 4px #ff5bb5,inset 0 0 24px rgba(255,90,180,.55)}.sq.g3{box-shadow:inset 0 0 0 4px #ffd230,inset 0 0 24px rgba(255,210,48,.55)}.sq.g4{box-shadow:inset 0 0 0 4px #ff5058,inset 0 0 20px rgba(255,80,88,.55)}.sq.g5{box-shadow:inset 0 0 0 4px #ffffff,inset 0 0 22px rgba(255,255,255,.55)}.sq.g6{box-shadow:inset 0 0 0 4px #b16cff,inset 0 0 20px rgba(177,108,255,.55)}.sq.g7{box-shadow:inset 0 0 0 4px #2fd78b,inset 0 0 18px rgba(47,215,139,.42)}.sq.g8{box-shadow:inset 0 0 0 4px #ff2525,inset 0 0 20px rgba(255,37,37,.52)}.sq.g9{box-shadow:inset 0 0 0 4px #ff9b28,inset 0 0 20px rgba(255,155,40,.52)}.sq.g10{box-shadow:inset 0 0 0 4px #42aaff,inset 0 0 18px rgba(66,170,255,.44)}.sq.g11{box-shadow:inset 0 0 0 4px #20bdf2,inset 0 0 20px rgba(32,189,242,.5)}.sq.gcount{box-shadow:inset 0 0 0 3px #59738d}.sq.gwarn{box-shadow:inset 0 0 0 4px #ff2b2b,inset 0 0 26px rgba(255,0,0,.65)}.sq.gamecell{font-size:15px}.custompaint{display:grid;grid-template-columns:repeat(8,1fr);gap:3px;max-width:520px;margin:8px auto}.paintcell{aspect-ratio:1;border:1px solid #3b4d68;border-radius:5px;cursor:pointer;touch-action:manipulation}.paletteBtn{width:30px;height:30px;border-radius:50%;border:2px solid #fff}.legend{display:flex;flex-wrap:wrap;gap:9px;font-size:11px;color:#a7b6cd}.dot{width:10px;height:10px;border-radius:50%;display:inline-block;margin-right:4px;vertical-align:-1px}.dgreen{background:#35f29b}.dblue{background:#42aaff}.dyellow{background:#ffd230}.dcyan{background:#19dcff}.dpink{background:#ff5bb5}@media(max-width:1100px){.grid{grid-template-columns:1fr}.gamegrid{grid-template-columns:repeat(2,1fr)}}@media(max-width:650px){header h1{font-size:20px}.wrap{padding:7px}.card{padding:10px}.map{grid-template-columns:1fr}.board{width:96vw}.sq{font-size:10px}button{flex:1}.gamegrid{grid-template-columns:1fr}.game p{min-height:0}}
</style></head>
<body><header><div style="display:flex;gap:10px;align-items:center;justify-content:space-between;flex-wrap:wrap"><div><h1 style="margin:0">Smart Chess V12 — Hardware Console</h1><div class="sub">Direct GPIO, sensors, mappings, LEDs, animations, calibration and advanced arcade games.</div></div><a class="primary" href="/app" style="text-decoration:none">OPEN V12 CHESS APP</a></div></header>
<div class="wrap">
<div class="stats"><div class="badge">Wi-Fi <span class="good">ONLINE</span></div><div class="badge">IP <b id="ip">...</b></div><div class="badge">Active <b id="count">0</b></div><div class="badge">Last <b id="last">--</b></div><div class="badge">Animation <b id="animState">OFF</b></div><div class="badge">Raw cells <b id="rawCells">0</b></div><div class="badge">Sensor <b id="diagBadge">...</b></div></div>
<div class="grid"><div>
<div class="card"><h2>8x8 Physical Board Mirror</h2><div class="note">Rank 8 is top, Rank 1 is bottom, A is left, H is right. This is the same coordinate system used by the physical board and games. H8 is active.</div><br><div id="board" class="board"></div><br><div class="legend"><span><i class="dot dgreen"></i>sensor</span><span><i class="dot dcyan"></i>piece 1</span><span><i class="dot dpink"></i>piece 2</span><span><i class="dot dyellow"></i>target</span><span><i class="dot dblue"></i>guide</span></div><br><div id="event" class="event">Waiting for sensor activity...</div></div>

<div class="card"><h2>14 Advanced Arcade Games</h2><div class="note">The 8x8 dashboard mirrors the same 64-cell game state shown by the physical LEDs. Your chess pieces are the controls. Reed switches detect magnet presence, not pressure, so every TAP/PRESS action means a quick lift and return on the SAME square. H8 is active.</div><br><div class="gamegrid">
<div class="game"><h3>Game 1 - Breakout / Arkanoid</h3><p>1 magnet on Rank 1. Slide left/right to move the one-square paddle. The white ball bounces and you must clear the purple bricks.</p><div class="gameMeta">Rank 1 control | L/R | 3 lives</div><button class="primary" onclick="startGame('breakout')">START</button></div>
<div class="game"><h3>Game 2 - Space Invaders</h3><p>1 magnet on Rank 1. Slide left/right to move the ship. TAP the same square to fire a missile at the descending aliens.</p><div class="gameMeta">Rank 1 control | TAP = fire</div><button class="primary" onclick="startGame('invaders')">START</button></div>
<div class="game"><h3>Game 3 - Frogger</h3><p>1 magnet on Rank 1. Move exactly one square UP, DOWN, LEFT or RIGHT. Cross the moving cars and reach Rank 8.</p><div class="gameMeta">1-square moves | reach top</div><button class="primary" onclick="startGame('frogger')">START</button></div>
<div class="game"><h3>Game 4 - Flappy Bird</h3><p>1 magnet anywhere. TAP the same square to flap upward. Move the magnet to change columns. Fly through the two-cell pipe gap.</p><div class="gameMeta">TAP = flap | move = steer</div><button class="primary" onclick="startGame('flappy')">START</button></div>
<div class="game"><h3>Game 5 - Pong</h3><p>2 magnets. P1 uses the A-file (A1-A8). P2 uses the H-file; H8 is active. Slide up/down to return the ball.</p><div class="gameMeta">2 players | first to 5</div><button class="primary" onclick="startGame('pong')">START</button></div>
<div class="game"><h3>Game 6 - Tron / Snake Versus</h3><p>2 magnets. Start P1 at D4 and P2 at E5. Move one square at a time. Every move leaves a permanent trail; hit a wall or any trail and you lose.</p><div class="gameMeta">2 players | trail collision</div><button class="primary" onclick="startGame('tron')">START</button></div>
<div class="game"><h3>Game 7 - Combat / Tank Battalion</h3><p>2 magnets on open maze squares. Move one square at a time. TAP a tank to fire a shell toward the other tank.</p><div class="gameMeta">2 tanks | maze | TAP = fire</div><button class="primary" onclick="startGame('tanks')">START</button></div>
<div class="game"><h3>Game 8 - Tetris</h3><p>1 magnet on Rank 1. Slide left/right to position the falling block. TAP the same square to rotate. Hold the magnet off the board to hard-drop.</p><div class="gameMeta">L/R | TAP = rotate | hold off = drop</div><button class="primary" onclick="startGame('tetris')">START</button></div>
<div class="game"><h3>Game 9 - Snake Solo</h3><p>1 magnet anywhere. Move one square at a time to steer the snake. Eat the food and avoid your tail and the wall.</p><div class="gameMeta">1-square moves | grow by eating</div><button class="primary" onclick="startGame('snake')">START</button></div>
<div class="game"><h3>Game 10 - Minesweeper</h3><p>2 magnets. P1 is the cursor: TAP the same square to reveal. P2 is the flag marker: place it on a hidden square to toggle a flag.</p><div class="gameMeta">2 pieces | reveal + flag</div><button class="primary" onclick="startGame('minesweeper')">START</button></div>
<div class="game"><h3>Game 11 - Whack-a-Mole</h3><p>1 magnet anywhere. A target flashes on the board. Move onto it or TAP it quickly. You have 30 seconds to score as many hits as possible.</p><div class="gameMeta">30 seconds | move/TAP to hit</div><button class="primary" onclick="startGame('whack')">START</button></div>
<div class="game"><h3>Game 12 - Lights Out</h3><p>One magnet navigates the board. TAP a square to toggle it and its four neighbors. Turn every lit tile off.</p><div class="gameMeta">1 magnet | move + TAP | logic puzzle</div><button class="primary" onclick="startGame('lightsout')">START</button></div>
<div class="game"><h3>Game 13 - Maze Runner</h3><p>Navigate a magnetic piece through a fixed maze. Walls are purple, the exit is yellow, and illegal wall moves are rejected.</p><div class="gameMeta">1 magnet | one-square movement | maze</div><button class="primary" onclick="startGame('maze')">START</button></div>
<div class="game"><h3>Game 14 - Memory Path</h3><p>Watch a growing sequence of squares, then move the magnet through the same sequence from memory. Three lives.</p><div class="gameMeta">1 magnet | sequence recall | 3 lives</div><button class="primary" onclick="startGame('memory')">START</button></div></div><br><div class="row"><button class="danger" onclick="stopGame()">STOP GAME</button></div><br><div class="stats"><div class="badge">Game <b id="gameActive">OFF</b></div><div class="badge">Mode <b id="gameMode">None</b></div><div class="badge">Score <b id="gameScore">0</b></div><div class="badge">Moves <b id="gameMoves">0</b></div><div class="badge">Lives <b id="gameLives">0</b></div><div class="badge">Target <b id="gameTarget">--</b></div></div><div id="gameStatus" class="event">No game running.</div></div>

<div class="card"><h2>64-Square Calibration</h2><div class="note">Every square is tested one by one. You will receive a prompt for each square. Each successful capture is hard-saved immediately.</div><div id="calStep" class="big" style="font-size:24px;font-weight:bold;margin:8px 0">A1 | 1 / 64</div><div id="calStatus" class="event">Calibration is not running.</div><div class="row"><button class="primary" onclick="startCalibration()">START CALIBRATION</button><button onclick="resumeCalibration()">RESUME SAVED</button><button class="danger" onclick="clearCalibration()">CLEAR</button></div><hr><div id="calSummary" class="event">No completed calibration yet.</div></div>

<div class="card"><h2>Sensor Diagnostic</h2><div class="note">With one magnet, you should see exactly 1 active physical row, 1 active physical column and 1 active cell.</div><div class="map"><div class="control"><div class="note">Physical rows</div><div id="rawRows" class="big">--------</div></div><div class="control"><div class="note">Physical columns</div><div id="rawCols" class="big">--------</div></div></div><br><div id="diagText" class="event">Ready</div></div>
</div>

<div>
<div class="card"><h2>GPIO Wiring Console</h2><div class="note">Change the actual ESP32 GPIO numbers used by the 8 row lines and 8 column lines. Save only after physically rewiring to match. The firmware validates duplicates and stores the pin map in flash.</div><div class="map"><div class="control"><div class="note">ROW GPIOs</div><div id="rowPinsEdit" class="map"></div></div><div class="control"><div class="note">COLUMN GPIOs</div><div id="colPinsEdit" class="map"></div></div></div><br><div class="row"><button class="primary" onclick="savePins()">SAVE GPIO WIRING</button><button onclick="reloadPins()">RELOAD</button></div><div id="pinsMsg" class="event">Hardware pin assignments loaded from ESP32.</div></div>

<div class="card"><h2>Row & Column Mapping</h2><div class="note">Change each logical row or column individually. A physical GPIO can only be used once. Selecting a GPIO already used by another position automatically swaps them.</div><hr><h3>Rows</h3><div id="rowMapping" class="map"></div><div class="row"><button onclick="reverseMapping('row')">REVERSE ROWS</button></div><hr><h3>Columns</h3><div id="colMapping" class="map"></div><div class="row"><button onclick="reverseMapping('col')">REVERSE COLUMNS</button></div><hr><div class="row"><button class="primary" onclick="saveMapping()">SAVE MAPPING</button><button onclick="reloadMapping()">RELOAD</button><button class="danger" onclick="defaultMapping()">DEFAULT</button></div><div id="mappingText" class="event">Loading...</div></div>

<div class="card"><h2>Magnet to LED Tracking</h2><div class="note">Move a magnet and the LED under its detected square follows it automatically.</div><br><button id="trackingBtn" class="primary" onclick="toggleTracking()">LED TRACKING: ON</button><hr><label>Sensitivity <b id="sensVal">10</b>/10</label><input id="sens" type="range" min="1" max="10" value="10" oninput="setSensitivity(this.value)"><label>Signal settle <b id="settleVal">8</b> us</label><input id="settle" type="range" min="2" max="100" value="8" oninput="setSettle(this.value)"></div>

<div class="card"><h2>LED Control</h2><div class="cols2"><div class="control"><div class="note">Selected LED</div><div id="selectedLED" class="big">LED 1</div><div class="row"><button onclick="prevLED()">< Prev</button><button onclick="nextLED()">Next ></button></div></div><div class="control"><div class="note">Manual color</div><input id="color" type="color" value="#ffffff" onchange="currentColor=this.value"><button onclick="applyLED()">APPLY LED</button></div></div><hr><button onclick="allOff()">ALL OFF</button><button onclick="allOn()">ALL ON</button><button onclick="matrixLED()">MATRIX STATUS</button><br><br><label>Brightness <b id="brightVal">80</b></label><input id="bright" type="range" min="1" max="255" value="80" oninput="setBrightness(this.value)"></div>

<div class="card"><h2>Animations</h2><div class="note">Each animation has its own color palette. RAINBOW uses 6 smoothly blended colors and loops seamlessly. The number of color pickers automatically matches that animation: Rainbow 6, Wave 2, Chase 3, Scan 3, Breath 1, Random 4, Center Out 2. Changing one animation does not overwrite another.</div><div class="row"><button onclick="anim('none')">OFF</button><button onclick="anim('rainbow')">RAINBOW</button><button onclick="anim('wave')">WAVE</button><button onclick="anim('chase')">CHASE</button><button onclick="anim('scan')">SCAN</button><button onclick="anim('breath')">BREATH</button><button onclick="anim('random')">RANDOM</button><button onclick="anim('centerout')" class="primary">CENTER OUT</button><button onclick="anim('custom')">CUSTOM</button></div><br><label>Speed <b id="speedVal">100</b></label><input id="speed" type="range" min="10" max="255" value="100" oninput="setSpeed(this.value)"><br><div id="animColors" class="cols2"></div><br><div class="row"><button class="primary" onclick="applyAnimPalette(false)">APPLY PALETTE</button><button class="primary" onclick="applyAnimPalette(true)">SAVE THIS ANIMATION COLORS</button><button onclick="hardSave()">HARD SAVE ALL</button></div></div>

<div class="card"><h2>Custom Animation Builder</h2><div class="note">Tap a palette color, then tap tiles directly. No add-color button. Repeat any frame into the remaining frames, name the animation, and save it in one of 8 flash slots.</div><div class="row"><label>Animation Slot <select id="customSlot"></select></label><label>Name <input id="customName" type="text" maxlength="18" value="My Animation" style="min-width:150px"></label><label>Repeat seconds <input id="customRepeat" type="number" min="0" max="600" value="0" style="width:95px"></label></div><div class="row"><label>Colors <select id="customColorCount" onchange="resizeCustomPalette()"><option>1</option><option>2</option><option>3</option><option selected>4</option><option>5</option><option>6</option><option>7</option><option>8</option></select></label><label>Frames <select id="customFrameCount" onchange="resizeCustomFrames()"><option selected>1</option><option>2</option><option>3</option><option>4</option><option>5</option><option>6</option><option>7</option><option>8</option></select></label><label>Frame ms <input id="customDelay" type="number" min="20" max="2000" value="120" style="width:90px"></label></div><div id="customPalette" class="row"></div><div class="row" id="customFrameButtons"></div><div id="customPaint" class="custompaint"></div><div class="row"><button onclick="clearCustomFrame()">CLEAR FRAME</button><button onclick="paintColor=0;renderCustomPalette()">ERASER</button><button onclick="repeatCurrentFrame()">REPEAT FRAME</button><button onclick="loadCustomSlot()">LOAD SLOT</button><button class="primary" onclick="saveCustomAnimation()">SAVE + PLAY</button><button onclick="deleteCustomAnimation()">DELETE SLOT</button></div><div id="customMsg" class="event">Custom builder ready.</div></div>

<div class="card"><h2>Piece / Move Colors</h2><div class="note">These colors control the square under detected magnets, the two player pieces, legal blue move guides, and red invalid-move warnings.</div><div class="cols2"><div class="control"><label>Normal square under piece</label><input id="trackColor" type="color"></div><div class="control"><label>Player 1</label><input id="p1Color" type="color"></div><div class="control"><label>Player 2</label><input id="p2Color" type="color"></div><div class="control"><label>Arcade guide</label><input id="guideColor" type="color"></div><div class="control"><label>Legal chess move (YELLOW)</label><input id="legalColor" type="color"></div><div class="control"><label>Lifted piece (BLUE)</label><input id="liftColor" type="color"></div><div class="control"><label>King danger / invalid move (RED)</label><input id="warningColor" type="color"></div></div><br><div class="row"><button class="primary" onclick="applyPieceColors(true)">SAVE PIECE COLORS</button></div></div>

<div class="card"><h2>64 LED Selection</h2><div id="ledGrid" class="map"></div></div>
<div class="card"><h2>Hardware Information</h2><div id="info" class="event">Loading...</div></div>
</div></div></div>
<script>let selectedLED=0,currentColor='#ffffff';const $=id=>document.getElementById(id);let lastPieceSig='';let custom={colorCount:4,frameCount:1,frameDelay:120,colors:['#00d2ff','#ff3cb4','#ffd228','#50ff78','#000000','#000000','#000000','#000000'],frames:Array.from({length:8},()=>Array(64).fill(0))},customFrame=0,paintColor=1,lastAnimSignature='';
function bitstr(m){let s='';for(let i=7;i>=0;i--)s+=(m&(1<<i))?'1':'0';return s}
function buildBoard(){let b=$('board');b.innerHTML='';for(let r=7;r>=0;r--)for(let c=0;c<8;c++){let d=document.createElement('div');d.className='sq';d.dataset.r=r;d.dataset.c=c;let n=String.fromCharCode(65+c)+(r+1);d.innerHTML='<span>'+n+'</span><span class="coord">'+n+'</span>';d.onclick=()=>{selectedLED=chessToLED(r,c);updateSelectedLED()};b.appendChild(d)}}
function chessToLED(r,c){return r%2===0?r*8+c:r*8+(7-c)}
function buildLEDGrid(){let g=$('ledGrid');g.innerHTML='';for(let i=0;i<64;i++){let d=document.createElement('div');d.className='control';d.style.padding='7px';d.style.textAlign='center';d.id='led'+i;d.innerText='LED '+(i+1);d.onclick=()=>{selectedLED=i;updateSelectedLED()};g.appendChild(d)}}
function updateSelectedLED(){$('selectedLED').innerText='LED '+(selectedLED+1);document.querySelectorAll('#ledGrid .control').forEach(x=>x.style.outline='none');let e=$('led'+selectedLED);if(e)e.style.outline='2px solid white'}
async function applyLED(){await fetch('/led?index='+selectedLED+'&color='+encodeURIComponent(currentColor));await update()}function prevLED(){selectedLED=(selectedLED+63)%64;updateSelectedLED()}function nextLED(){selectedLED=(selectedLED+1)%64;updateSelectedLED()}
async function allOff(){await fetch('/alloff');await update()}async function allOn(){await fetch('/allon?color='+encodeURIComponent(currentColor));await update()}async function matrixLED(){await fetch('/matrix');await update()}
async function setBrightness(v){$('brightVal').innerText=v;await fetch('/brightness?value='+v)}async function setSpeed(v){$('speedVal').innerText=v;await fetch('/speed?value='+v)}
const ANIM_COUNTS={RAINBOW:6,WAVE:2,CHASE:3,SCAN:3,BREATH:1,RANDOM:4,'CENTER OUT':2};
function renderAnimColors(d){let count=d.animationColorCount||0,arr=Array.isArray(d.animationColors)?d.animationColors:[];if(!count)return;$('animColors').innerHTML='';for(let i=0;i<count;i++){let w=document.createElement('div');w.className='control';w.innerHTML='<label>Color '+(i+1)+'</label><input id="animc'+i+'" type="color" value="'+(arr[i]||'#ffffff')+'" onchange="applyAnimPalette(false)">';$('animColors').appendChild(w)}}
function readAnimColors(){let a=[];for(let i=0;i<8;i++){let e=$('animc'+i);if(e)a.push(e.value)}return a}
async function applyAnimPalette(save){let colors=readAnimColors();if(!colors.length)return;let u='/animation/colors?colors='+encodeURIComponent(colors.join(','));if(save)u+='&save=1';let r=await fetch(u);let t=await r.text();if(!r.ok)alert(t);await update()}
async function anim(n){let r=await fetch('/animation?mode='+n);let t=await r.text();if(!r.ok)alert(t);if(n==='custom')customFrame=0;await update()}
async function hardSave(){await applyAnimPalette(false);await applyPieceColors(false);await fetch('/settings/save');alert('All settings hard-saved to ESP32 flash.');await update()}
async function toggleTracking(){await fetch('/tracking');await update()}async function setSensitivity(v){$('sensVal').innerText=v;await fetch('/sensitivity?value='+v)}async function setSettle(v){$('settleVal').innerText=v;await fetch('/settle?value='+v)}
async function reloadPins(){try{let d=await (await fetch('/pins')).json();let rh='',ch='';for(let i=0;i<8;i++){rh+='<div class="mapitem"><b>R'+(i+1)+'</b><input id="rp'+i+'" type="number" min="0" max="39" value="'+d.rowPins[i]+'"></div>';ch+='<div class="mapitem"><b>C'+(i+1)+'</b><input id="cp'+i+'" type="number" min="0" max="39" value="'+d.colPins[i]+'"></div>'}$('rowPinsEdit').innerHTML=rh;$('colPinsEdit').innerHTML=ch;}catch(e){set('pinsMsg','Unable to read GPIO assignments.','error')}}
async function savePins(){let r=[],c=[];for(let i=0;i<8;i++){r.push(parseInt($('rp'+i).value));c.push(parseInt($('cp'+i).value))}if(new Set(r).size!==8||new Set(c).size!==8||r.some(x=>!Number.isInteger(x))||c.some(x=>!Number.isInteger(x))){alert('Use eight unique row GPIOs and eight unique column GPIOs.');return}let x=await fetch('/pins/set?rows='+r.join(',')+'&cols='+c.join(','));let t=await x.text();set('pinsMsg',t,x.ok?'success':'error');if(x.ok){await reloadPins();await reloadMapping();}}
function renderMap(d){let rh='',ch='';for(let i=0;i<8;i++){rh+='<div class="mapitem"><b>Row '+(i+1)+'</b><select id="rowSel'+i+'">'+opts(d.rowMap[i],d.rowPins)+'</select></div>';ch+='<div class="mapitem"><b>Col '+(i+1)+'</b><select id="colSel'+i+'">'+opts(d.colMap[i],d.colPins)+'</select></div>'}$('rowMapping').innerHTML=rh;$('colMapping').innerHTML=ch;for(let i=0;i<8;i++){$('rowSel'+i).value=d.rowMap[i];$('colSel'+i).value=d.colMap[i];$('rowSel'+i).onchange=()=>swapMap('row',i);$('colSel'+i).onchange=()=>swapMap('col',i)}}
function opts(sel,pins){let s='';for(let i=0;i<8;i++)s+='<option value="'+i+'">Physical '+(i+1)+' - GPIO '+pins[i]+'</option>';return s}
function swapMap(axis,i){let p=axis==='row'?'rowSel':'colSel',s=$(p+i),old=parseInt(s.dataset.last??s.value),nv=parseInt(s.value);for(let j=0;j<8;j++){if(j!==i&&parseInt($(p+j).value)===nv){$(p+j).value=old;$(p+j).dataset.last=old}}s.dataset.last=nv;previewMap()}
function previewMap(){let a=[],b=[];for(let i=0;i<8;i++){a.push('R'+(i+1)+'='+$('rowSel'+i).value);b.push('C'+(i+1)+'='+$('colSel'+i).value)}$('mappingText').innerText=a.join('  ')+'\n'+b.join('  ')+'\nPress SAVE MAPPING.'}
async function reloadMapping(){try{renderMap(await (await fetch('/mapping')).json())}catch(e){}}async function saveMapping(){let r=[],c=[];for(let i=0;i<8;i++){r.push(parseInt($('rowSel'+i).value));c.push(parseInt($('colSel'+i).value))}if(new Set(r).size!==8||new Set(c).size!==8){alert('Each physical row and column must be used exactly once.');return}let x=await fetch('/mapping/set?rows='+r.join(',')+'&cols='+c.join(','));if(!x.ok){alert(await x.text());return}await reloadMapping();alert('Mapping saved to ESP32 flash.')}async function reverseMapping(a){await fetch('/mapping/reverse?axis='+a);await reloadMapping()}async function defaultMapping(){await fetch('/mapping/reset');await reloadMapping()}
async function startGame(type){let r=await fetch('/game/start?type='+type);let t=await r.text();await update();if(/failed|place exactly/i.test(t))alert(t)}async function stopGame(){await fetch('/game/stop');await update()}
async function startCalibration(){if(!confirm('Start 64-square calibration?\n\nUse ONE magnet. A prompt will tell you every square.'))return;await fetch('/calibration/reset');await runCalPrompts()}async function resumeCalibration(){if(!confirm('Resume saved calibration?'))return;await fetch('/calibration/resume');await runCalPrompts()}async function clearCalibration(){if(!confirm('Clear all saved calibration progress?'))return;await fetch('/calibration/clear');await update();await reloadMapping()}async function runCalPrompts(){while(true){let d=await (await fetch('/status')).json();await update();if(!d.calibrationRunning)break;let ok=confirm('CALIBRATION '+d.calibrationStepNumber+' / 64\n\nPlace ONE magnet on '+d.calibrationStepName+'\n\nPress OK to capture.');if(!ok){await fetch('/calibration/pause');await update();break}let x=await fetch('/calibration/capture');let t=await x.text();if(/failed|mismatch|rejected/i.test(t))alert(t)}}
function gameSymbol(code){if(code===1)return 'P';if(code===2)return 'Q';if(code===3)return '*';if(code===4)return 'E';if(code===5)return 'o';if(code===6)return '#';if(code===7)return '+';if(code===8)return 'X';if(code===9)return 'F';if(code===10)return '.';if(code===11)return '_';if(code===12)return '!';if(code>=20&&code<=28){let n=code-20;return n===0?'':String(n)}return ''}
function drawGame(d){let cells=document.querySelectorAll('#board .sq'),grid=Array.isArray(d.gameGrid)?d.gameGrid:[];cells.forEach(e=>{e.className='sq';let r=+e.dataset.r,c=+e.dataset.c,idx=r*8+c,code=grid[idx]??0;if(d.gameMode==='None')code=0;if(d.gameMode!=='None'&&code>0){e.classList.add('gamecell');if(code===8)e.classList.add('gwarn');else if(code<20)e.classList.add('g'+code);else e.classList.add('gcount')}else if(d.matrix?.[r]?.[c])e.classList.add('on');let first=e.querySelector('span');if(first)first.textContent=code?gameSymbol(code):(String.fromCharCode(65+c)+(r+1));});}
function applyPieceColorInputs(d){$('trackColor').value=d.trackingColor||'#00ff50';$('p1Color').value=d.gameP1Color||'#00d2ff';$('p2Color').value=d.gameP2Color||'#ff50b4';$('guideColor').value=d.gameGuideColor||'#005abf';$('legalColor').value=d.gameLegalColor||'#ffd230';$('liftColor').value=d.gameLiftColor||'#42aaff';$('warningColor').value=d.gameWarningColor||'#ff0000'}
async function applyPieceColors(save){let u='/piece/colors?tracking='+encodeURIComponent($('trackColor').value)+'&p1='+encodeURIComponent($('p1Color').value)+'&p2='+encodeURIComponent($('p2Color').value)+'&guide='+encodeURIComponent($('guideColor').value)+'&legal='+encodeURIComponent($('legalColor').value)+'&lift='+encodeURIComponent($('liftColor').value)+'&warning='+encodeURIComponent($('warningColor').value);if(save)u+='&save=1';let r=await fetch(u);let t=await r.text();if(!r.ok)alert(t);await update()}
function customPaletteCount(){return parseInt($('customColorCount').value)}function customFrameCountValue(){return parseInt($('customFrameCount').value)}
function renderCustomPalette(){let c=customPaletteCount();$('customPalette').innerHTML='';for(let i=0;i<c;i++){let w=document.createElement('button');w.type='button';w.className='paletteBtn'+(paintColor===i+1?' active':'');w.style.background=custom.colors[i]||'#ffffff';w.title='Color '+(i+1);w.onclick=()=>{paintColor=i+1;renderCustomPalette();};$('customPalette').appendChild(w)}}
function renderCustomFrameButtons(){let n=customFrameCountValue();$('customFrameButtons').innerHTML='';for(let i=0;i<n;i++){let b=document.createElement('button');b.innerText='FRAME '+(i+1);b.className=i===customFrame?'primary':'';b.onclick=()=>{customFrame=i;renderCustomPaint();renderCustomFrameButtons()};$('customFrameButtons').appendChild(b)}}
function renderCustomPaint(){let f=custom.frames[customFrame];$('customPaint').innerHTML='';for(let r=7;r>=0;r--)for(let c=0;c<8;c++){let i=r*8+c,d=document.createElement('button');d.type='button';d.className='paintcell';let v=f[i]||0;d.style.background=v?custom.colors[v-1]:'#05080d';d.title=String.fromCharCode(65+c)+(r+1);d.onclick=()=>{custom.frames[customFrame][i]=paintColor||0;renderCustomPaint()};$('customPaint').appendChild(d)}}
function repeatCurrentFrame(){if(custom.frameCount<2)return;const src=custom.frames[customFrame].slice();for(let i=customFrame+1;i<custom.frameCount;i++)custom.frames[i]=src.slice();renderCustomFrameButtons();renderCustomPaint();$('customMsg').innerText='Frame '+(customFrame+1)+' repeated through the remaining frames.';}
function clearCustomFrame(){custom.frames[customFrame]=Array(64).fill(0);renderCustomPaint()}
function addPattern(){clearCustomFrame()}
function resizeCustomPalette(){let c=customPaletteCount();custom.colorCount=c;for(let f=0;f<8;f++)for(let i=0;i<64;i++)if(custom.frames[f][i]>c)custom.frames[f][i]=0;paintColor=Math.max(1,Math.min(paintColor||1,c));renderCustomPalette();renderCustomPaint()}
function resizeCustomFrames(){let n=customFrameCountValue();custom.frameCount=n;while(custom.frames.length<8)custom.frames.push(Array(64).fill(0));customFrame=Math.min(customFrame,n-1);renderCustomFrameButtons();renderCustomPaint()}
async function loadCustom(){try{let d=await json(boardURL()+'/custom');custom.colorCount=d.colorCount||4;custom.frameCount=d.frameCount||1;custom.frameDelay=d.frameDelay||120;custom.repeatSeconds=d.repeatSeconds||0;custom.colors=(d.colors||[]).slice(0,custom.colorCount);custom.frames=(d.frames||[]).slice(0,8).map(s=>String(s).padEnd(64,'0').slice(0,64).split('').map(x=>parseInt(x)));while(custom.frames.length<8)custom.frames.push(Array(64).fill(0));$('customColorCount').value=custom.colorCount;$('customFrameCount').value=custom.frameCount;$('customDelay').value=custom.frameDelay;$('customRepeat').value=custom.repeatSeconds;$('customName').value=d.activeName||'My Animation';customFrame=0;paintColor=1;renderCustomPalette();renderCustomFrameButtons();renderCustomPaint();await loadCustomSlots();}catch(e){setMessage('customMsg','Custom animation storage not available: '+e.message,'error')}}
async function loadCustomSlots(){try{const slots=await json(boardURL()+'/custom/slots');const sel=$('customSlot');sel.innerHTML='';(slots.slots||[]).forEach((x,i)=>{const o=document.createElement('option');o.value=i;o.textContent=x.used?x.name:('Slot '+(i+1));sel.appendChild(o)});sel.value=String(slots.activeSlot||0);$('customName').value=slots.activeName||'My Animation';}catch(e){}}
async function loadCustomSlot(){const slot=Number($('customSlot').value||0);try{let d=await json(boardURL()+'/custom/slot/load?slot='+slot);custom.colorCount=d.colorCount||4;custom.frameCount=d.frameCount||1;custom.frameDelay=d.frameDelay||120;custom.repeatSeconds=d.repeatSeconds||0;custom.colors=(d.colors||[]).slice(0,custom.colorCount);custom.frames=(d.frames||[]).slice(0,8).map(s=>String(s).padEnd(64,'0').slice(0,64).split('').map(x=>parseInt(x)));while(custom.frames.length<8)custom.frames.push(Array(64).fill(0));$('customColorCount').value=custom.colorCount;$('customFrameCount').value=custom.frameCount;$('customDelay').value=custom.frameDelay;$('customRepeat').value=custom.repeatSeconds;$('customName').value=d.name||('Slot '+(slot+1));customFrame=0;paintColor=1;renderCustomPalette();renderCustomFrameButtons();renderCustomPaint();$('customMsg').innerText='Loaded '+$('customName').value+'.';await anim('custom');}catch(e){$('customMsg').innerText='Slot '+(slot+1)+' is empty. Build it and save.'}}
async function saveCustomAnimation(){custom.colorCount=customPaletteCount();custom.frameCount=customFrameCountValue();custom.frameDelay=Math.max(20,Math.min(2000,parseInt($('customDelay').value)||120));custom.repeatSeconds=Math.max(0,Math.min(600,parseInt($('customRepeat').value)||0));custom.colors=custom.colors.slice(0,custom.colorCount);let colorList=custom.colors.join(',');let frames=custom.frames.slice(0,custom.frameCount).map(a=>a.join(''));let slot=Number($('customSlot').value||0);let name=(($('customName').value||'My Animation').trim().slice(0,18)||('Slot '+(slot+1)));let body=[slot,name.replaceAll('|',' '),custom.repeatSeconds,custom.frameCount,custom.frameDelay,custom.colorCount,colorList,...frames].join('|');let r=await fetch(boardURL()+'/custom/slot/save',{method:'POST',headers:{'Content-Type':'text/plain'},body});let t=await r.text();$('customMsg').innerText=t;if(r.ok){await loadCustomSlot();toast('Custom animation saved to '+name+'.','success')}}
async function deleteCustomAnimation(){const slot=Number($('customSlot').value||0);if(!confirm('Delete saved animation in this slot?'))return;let r=await fetch(boardURL()+'/custom/slot/delete?slot='+slot);let t=await r.text();$('customMsg').innerText=t;await loadCustomSlots();await loadCustom()}
async function update(){try{let d=await (await fetch('/status')).json();$('ip').innerText=d.ip;$('count').innerText=d.active;$('last').innerText=d.lastSquare;$('rawCells').innerText=d.rawActiveCells;$('event').innerText=d.lastEvent;$('rawRows').innerText=bitstr(d.rawRowMask);$('rawCols').innerText=bitstr(d.rawColMask);$('diagText').innerText=d.diagnosticMessage;$('diagBadge').innerText=d.multipleRawDetected?'MULTIPLE':'OK';$('diagBadge').className=d.multipleRawDetected?'bad':'good';$('sensVal').innerText=d.sensitivity;$('settleVal').innerText=d.settleUs;$('brightVal').innerText=d.brightness;$('bright').value=d.brightness;$('speedVal').innerText=d.speed;$('speed').value=d.speed;$('trackingBtn').innerText=d.tracking?'LED TRACKING: ON':'LED TRACKING: OFF';$('animState').innerText=d.animation||'OFF';let sig=(d.animation||'')+'|'+(d.animationColorCount||0)+'|'+(d.animationColors||[]).join(',');if(sig!==lastAnimSignature){lastAnimSignature=sig;renderAnimColors(d)}let pcs=(d.trackingColor||'')+'|'+(d.gameP1Color||'')+'|'+(d.gameP2Color||'')+'|'+(d.gameGuideColor||'')+'|'+(d.gameLegalColor||'')+'|'+(d.gameLiftColor||'')+'|'+(d.gameWarningColor||'');if(pcs!==lastPieceSig){lastPieceSig=pcs;applyPieceColorInputs(d)}$('gameActive').innerText=d.gameActive?'ON':'OFF';$('gameMode').innerText=d.gameMode;$('gameScore').innerText=d.gameScore;$('gameMoves').innerText=d.gameMoves;$('gameLives').innerText=d.gameLives;$('gameTarget').innerText=d.gameTarget;$('gameStatus').innerText=d.gameStatus;$('calStatus').innerText=d.calibrationStatus;$('calSummary').innerText=d.calibrationSummary;$('calStep').innerText=d.calibrationStepName+' | '+d.calibrationStepNumber+' / 64';$('info').innerHTML='Board: 8x8<br>LED GPIO: 2<br>Rows: '+d.rowPins.join(', ')+'<br>Columns: '+d.colPins.join(', ')+'<br>Row map: '+d.rowText+'<br>Column map: '+d.colText+'<br>H8: ACTIVE';drawGame(d)}catch(e){$('event').innerText='Connection lost - reconnecting...'}}
$("customSlot")?.addEventListener("change",loadCustomSlot);$("customDelay")?.addEventListener("input",()=>{});$("customRepeat")?.addEventListener("input",()=>{});buildBoard();buildLEDGrid();updateSelectedLED();reloadMapping();reloadPins();loadCustom();loadCustomSlots();update();setInterval(update,350);
</script></body></html>

)rawliteral";

// ============================================================
// HTTP: Main page
// ============================================================

void handleRoot() {
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0", true);
  server.sendHeader("Pragma", "no-cache", true);
  server.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
}

void handleHardware() {
  handleRoot();
}

// ============================================================
// HTTP: LED
// ============================================================

void handleLED() {
  if (!server.hasArg("index") || !server.hasArg("color")) {
    server.send(400, "text/plain", "Missing parameters");
    return;
  }

  int index = server.arg("index").toInt();
  String color = server.arg("color");

  if (index < 0 || index >= NUM_LEDS) {
    server.send(400, "text/plain", "Invalid LED");
    return;
  }

  if (color.length() != 7 || color.charAt(0) != '#') {
    server.send(400, "text/plain", "Invalid color");
    return;
  }

  long number = strtol(color.substring(1).c_str(), NULL, 16);
  uint8_t r = (number >> 16) & 0xFF;
  uint8_t g = (number >> 8) & 0xFF;
  uint8_t b = number & 0xFF;

  animationMode = ANIM_NONE;
  leds[index] = CRGB(r, g, b);
  FastLED.show();

  server.send(200, "text/plain", "LED updated");
}

void handleAllOff() {
  animationMode = ANIM_NONE;
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  server.send(200, "text/plain", "All LEDs OFF");
}

void handleAllOn() {
  if (!server.hasArg("color")) {
    server.send(400, "text/plain", "Missing color");
    return;
  }

  String color = server.arg("color");
  if (color.length() != 7 || color.charAt(0) != '#') {
    server.send(400, "text/plain", "Invalid color");
    return;
  }

  long number = strtol(color.substring(1).c_str(), NULL, 16);
  uint8_t r = (number >> 16) & 0xFF;
  uint8_t g = (number >> 8) & 0xFF;
  uint8_t b = number & 0xFF;

  animationMode = ANIM_NONE;
  fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
  server.send(200, "text/plain", "All LEDs ON");
}

void handleMatrix() {
  animationMode = ANIM_NONE;
  drawMatrixStatus();
  server.send(200, "text/plain", "Matrix status displayed");
}

void handleBrightness() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing value");
    return;
  }

  int value = constrain(server.arg("value").toInt(), 1, 255);
  brightness = (uint8_t)value;
  FastLED.setBrightness(brightness);
  FastLED.show();
  saveAllSettings();
  server.send(200, "text/plain", String(brightness));
}

void handleSpeed() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing value");
    return;
  }

  int value = constrain(server.arg("value").toInt(), 10, 255);
  animationSpeed = (uint8_t)value;
  saveAllSettings();
  server.send(200, "text/plain", String(value));
}

void renderAnimationImmediately() {
  lastAnimationUpdate = 0;
  if (animationMode == ANIM_NONE || gameActive) return;
  switch (animationMode) {
    case ANIM_RAINBOW: animationRainbow(); break;
    case ANIM_WAVE: animationWave(); break;
    case ANIM_CHASE: animationChase(); break;
    case ANIM_SCAN: animationScan(); break;
    case ANIM_BREATH: animationBreath(); break;
    case ANIM_RANDOM: animationRandom(); break;
    case ANIM_CENTER_OUT: animationCenterOut(); break;
    case ANIM_CUSTOM: animationCustom(); break;
    default: break;
  }
  lastAnimationUpdate = millis();
}

void handleAnimation() {
  if (!server.hasArg("mode")) { server.send(400, "text/plain", "Missing mode"); return; }
  String mode = server.arg("mode"); mode.toLowerCase();
  if (mode == "none") {
    animationMode = ANIM_NONE; clearLEDs();
    if (!gameActive && pieceTrackingEnabled) updatePieceTrackingLEDs();
    server.send(200, "text/plain", "Animation OFF"); return;
  }
  if (mode == "rainbow") animationMode = ANIM_RAINBOW;
  else if (mode == "wave") animationMode = ANIM_WAVE;
  else if (mode == "chase") animationMode = ANIM_CHASE;
  else if (mode == "scan") animationMode = ANIM_SCAN;
  else if (mode == "breath") animationMode = ANIM_BREATH;
  else if (mode == "random") animationMode = ANIM_RANDOM;
  else if (mode == "centerout" || mode == "center-out" || mode == "center") animationMode = ANIM_CENTER_OUT;
  else if (mode == "custom") animationMode = ANIM_CUSTOM;
  else { server.send(400, "text/plain", "Unknown animation mode"); return; }

  // Restart animated modes from a clean phase so selecting RAINBOW always
  // begins immediately at the start of its smooth color cycle.
  animationHue = 0;
  if (animationMode == ANIM_CUSTOM) { customAnimationFrame = 0; customAnimationStartedAt = millis(); }
  syncCurrentAnimationColors();
  renderAnimationImmediately();
  server.send(200, "text/plain", "Animation: " + animationModeName());
}

void parseColorList(const String &list, CRGB *outColors, uint8_t &count, uint8_t maxCount) {
  count = 0;
  int start = 0;
  while (start <= (int)list.length() && count < maxCount) {
    int comma = list.indexOf(',', start);
    String token = comma < 0 ? list.substring(start) : list.substring(start, comma);
    token.trim();
    CRGB col;
    if (parseHexColor(token, col)) outColors[count++] = col;
    if (comma < 0) break;
    start = comma + 1;
  }
}

void handleAnimationColors() {
  if (!server.hasArg("colors")) {
    // Keep backward compatibility for older dashboard links.
    if (server.hasArg("color1")) server.arg("color1");
    else { server.send(400, "text/plain", "Missing colors"); return; }
  }

  if (animationMode >= ANIM_RAINBOW && animationMode <= ANIM_CENTER_OUT) {
    uint8_t p = animationPaletteIndex(animationMode);
    uint8_t expected = animationPaletteCounts[p];
    CRGB parsed[MAX_ANIMATION_COLORS];
    uint8_t count = 0;
    if (server.hasArg("colors")) parseColorList(server.arg("colors"), parsed, count, MAX_ANIMATION_COLORS);
    else {
      if (server.hasArg("color1")) { CRGB x; if (parseHexColor(server.arg("color1"),x)) parsed[count++]=x; }
      if (server.hasArg("color2")) { CRGB x; if (parseHexColor(server.arg("color2"),x)) parsed[count++]=x; }
    }
    if (count != expected) { server.send(400, "text/plain", "This animation needs exactly " + String(expected) + " colors."); return; }
    for (uint8_t i = 0; i < expected; i++) animationPalettes[p][i] = parsed[i];
    syncCurrentAnimationColors();
    if (server.hasArg("save") && server.arg("save") == "1") saveAnimationPalettes();
    renderAnimationImmediately();
    server.send(200, "text/plain", "Animation palette updated for " + animationModeName());
    return;
  }

  server.send(400, "text/plain", "Select a built-in animation before editing its colors.");
}

void handlePieceColors() {
  if (server.hasArg("tracking")) {
    CRGB c; if (!parseHexColor(server.arg("tracking"), c)) { server.send(400,"text/plain","Invalid tracking color"); return; }
    pieceTrackingColor = c;
  }
  if (server.hasArg("p1")) {
    CRGB c; if (!parseHexColor(server.arg("p1"), c)) { server.send(400,"text/plain","Invalid P1 color"); return; }
    gameP1Color = c;
  }
  if (server.hasArg("p2")) {
    CRGB c; if (!parseHexColor(server.arg("p2"), c)) { server.send(400,"text/plain","Invalid P2 color"); return; }
    gameP2Color = c;
  }
  if (server.hasArg("guide")) {
    CRGB c; if (!parseHexColor(server.arg("guide"), c)) { server.send(400,"text/plain","Invalid guide color"); return; }
    gameGuideColor = c;
  }
  if (server.hasArg("legal")) {
    CRGB c; if (!parseHexColor(server.arg("legal"), c)) { server.send(400,"text/plain","Invalid legal color"); return; }
    gameLegalColor = c;
  }
  if (server.hasArg("lift")) {
    CRGB c; if (!parseHexColor(server.arg("lift"), c)) { server.send(400,"text/plain","Invalid lift color"); return; }
    gameLiftColor = c;
  }
  if (server.hasArg("warning")) {
    CRGB c; if (!parseHexColor(server.arg("warning"), c)) { server.send(400,"text/plain","Invalid warning color"); return; }
    gameWarningColor = c;
  }
  if (server.hasArg("save") && server.arg("save") == "1") saveGameColors();
  if (gameActive) gameRenderLEDs(); else if (animationMode != ANIM_NONE) renderAnimationImmediately(); else if (pieceTrackingEnabled) updatePieceTrackingLEDs();
  server.send(200,"text/plain","Piece/game colors updated");
}

void handleHardSave() {
  saveAllSettings();
  saveMapping();
  savePinConfig();
  saveGameColors();
  server.send(200, "text/plain", "All settings hard-saved to ESP32 flash");
}

// ============================================================
// HTTP: Custom user animation builder
// ============================================================
void handleCustomGet() {
  String json = "{\"colorCount\":" + String(customAnimationColorCount) + ",\"frameCount\":" + String(customAnimationFrameCount) + ",\"frameDelay\":" + String(customAnimationFrameDelay) + ",\"repeatSeconds\":" + String(customAnimationRepeatSeconds) + ",\"activeSlot\":" + String(customActiveSlot) + ",\"activeName\":\"" + jsonEscape(customSlotName(customActiveSlot)) + "\",\"colors\":[";
  for (uint8_t i = 0; i < CUSTOM_MAX_COLORS; i++) {
    char b[8]; snprintf(b,sizeof(b),"#%02X%02X%02X",customAnimationColors[i].r,customAnimationColors[i].g,customAnimationColors[i].b);
    json += "\"" + String(b) + "\""; if (i < CUSTOM_MAX_COLORS-1) json += ",";
  }
  json += "],\"frames\":[";
  for (uint8_t f = 0; f < CUSTOM_MAX_FRAMES; f++) {
    json += "\"";
    for (uint8_t i = 0; i < 64; i++) json += char('0' + constrain((int)customAnimationFrames[f][i],0,8));
    json += "\"";
    if (f < CUSTOM_MAX_FRAMES-1) json += ",";
  }
  json += "]}";
  server.send(200,"application/json",json);
}

String customTakeField(const String &body, int &pos) {
  int sep = body.indexOf('|', pos);
  if (sep < 0) { String x = body.substring(pos); pos = body.length(); return x; }
  String x = body.substring(pos, sep); pos = sep + 1; return x;
}

bool parseHex6Token(const String &token, CRGB &out) {
  String t = token; t.trim(); if (t.charAt(0) != '#') t = "#" + t; return parseHexColor(t,out);
}

void handleCustomSave() {
  String body = server.arg("plain");
  if (body.length() < 10) { server.send(400,"text/plain","Custom animation data is empty."); return; }

  int pos = 0;
  String fcs = customTakeField(body,pos);
  String dly = customTakeField(body,pos);
  String ccs = customTakeField(body,pos);
  String colors = customTakeField(body,pos);
  uint8_t fc = (uint8_t)constrain(fcs.toInt(),1,(int)CUSTOM_MAX_FRAMES);
  uint16_t fd = (uint16_t)constrain(dly.toInt(),20,2000);
  uint8_t cc = (uint8_t)constrain(ccs.toInt(),1,(int)CUSTOM_MAX_COLORS);

  CRGB parsedColors[CUSTOM_MAX_COLORS];
  uint8_t parsedCount=0;
  int cp=0;
  while (cp <= (int)colors.length() && parsedCount < cc) {
    int comma = colors.indexOf(',',cp);
    String token = comma<0 ? colors.substring(cp) : colors.substring(cp,comma);
    CRGB col; if (!parseHex6Token(token,col)) { server.send(400,"text/plain","Invalid custom palette color."); return; }
    parsedColors[parsedCount++] = col;
    if (comma<0) break; cp=comma+1;
  }
  if (parsedCount != cc) { server.send(400,"text/plain","Custom palette color count mismatch."); return; }

  uint8_t parsedFrames[CUSTOM_MAX_FRAMES][64]; memset(parsedFrames,0,sizeof(parsedFrames));
  for (uint8_t f = 0; f < fc; f++) {
    if (pos >= (int)body.length()) { server.send(400,"text/plain","Missing custom frame data."); return; }
    String frame = customTakeField(body,pos);
    if (frame.length() != 64) { server.send(400,"text/plain","Each custom frame must contain 64 cells."); return; }
    for (uint8_t i = 0; i < 64; i++) {
      char ch = frame.charAt(i);
      if (ch < '0' || ch > '8') { server.send(400,"text/plain","Custom frame cells must be 0..8."); return; }
      uint8_t idx = (uint8_t)(ch - '0');
      if (idx > cc) { server.send(400,"text/plain","A frame uses a color that is outside the selected palette."); return; }
      parsedFrames[f][i] = idx;
    }
  }

  customAnimationColorCount = cc;
  customAnimationFrameCount = fc;
  customAnimationFrameDelay = fd;
  customAnimationRepeatSeconds = constrain(customAnimationRepeatSeconds, (uint16_t)0, (uint16_t)600);
  customActiveSlot = 0;
  for (uint8_t i = 0; i < 8; ++i) { customAnimationColors[i] = parsedColors[i]; }
  memcpy(customAnimationFrames, parsedFrames, sizeof(parsedFrames));
  customAnimationFrame = 0;
  saveAnimationPalettes();
  if (animationMode == ANIM_CUSTOM && !gameActive) renderAnimationImmediately();
  server.send(200,"text/plain","Custom animation saved to ESP32 flash.");
}

// ============================================================
// HTTP: Named custom animation slots
// ============================================================
void handleCustomSlots() {
  String json="{\"activeSlot\":"+String(customActiveSlot)+",\"activeName\":\""+jsonEscape(customSlotName(customActiveSlot))+"\",\"slots\":[";
  for(uint8_t i=0;i<CUSTOM_SLOT_COUNT;i++){CustomSlotMeta meta{};prefs.begin("smartchess",true);char key[12];snprintf(key,sizeof(key),"s%umeta",i);if(prefs.getBytesLength(key)==sizeof(meta))prefs.getBytes(key,&meta,sizeof(meta));prefs.end();json+="{\"used\":"+(String(meta.used?"true":"false"))+",\"name\":\""+jsonEscape(String(meta.used?meta.name:"Slot "+String(i+1)))+"\"}";if(i<CUSTOM_SLOT_COUNT-1)json+=",";}
  json+="]}";server.send(200,"application/json",json);
}

void handleCustomSlotLoad() {
  if(!server.hasArg("slot")){server.send(400,"text/plain","Missing slot.");return;}uint8_t slot=(uint8_t)constrain(server.arg("slot").toInt(),0,(int)CUSTOM_SLOT_COUNT-1);
  if(!loadCustomSlotFromNVS(slot)){server.send(404,"text/plain","That custom slot is empty.");return;}
  String json="{\"slot\":"+String(customActiveSlot)+",\"name\":\""+jsonEscape(customSlotName(slot))+"\",\"colorCount\":"+String(customAnimationColorCount)+",\"frameCount\":"+String(customAnimationFrameCount)+",\"frameDelay\":"+String(customAnimationFrameDelay)+",\"repeatSeconds\":"+String(customAnimationRepeatSeconds)+",\"colors\":[";
  for(uint8_t i=0;i<CUSTOM_MAX_COLORS;i++){char b[8];snprintf(b,sizeof(b),"#%02X%02X%02X",customAnimationColors[i].r,customAnimationColors[i].g,customAnimationColors[i].b);json+="\""+String(b)+"\"";if(i<CUSTOM_MAX_COLORS-1)json+=",";}
  json+="] ,\"frames\":[";for(uint8_t f=0;f<CUSTOM_MAX_FRAMES;f++){json+="\"";for(uint8_t i=0;i<64;i++)json+=char('0'+constrain((int)customAnimationFrames[f][i],0,8));json+="\"";if(f<CUSTOM_MAX_FRAMES-1)json+=",";}json+="]}";server.send(200,"application/json",json);
}

void handleCustomSlotSave() {
  String body=server.arg("plain");int pos=0;String slotS=customTakeField(body,pos);String name=customTakeField(body,pos);String repeatS=customTakeField(body,pos);String fcs=customTakeField(body,pos);String dly=customTakeField(body,pos);String ccs=customTakeField(body,pos);String colors=customTakeField(body,pos);
  uint8_t slot=(uint8_t)constrain(slotS.toInt(),0,(int)CUSTOM_SLOT_COUNT-1);uint16_t repeat=(uint16_t)constrain(repeatS.toInt(),0,600);uint8_t fc=(uint8_t)constrain(fcs.toInt(),1,(int)CUSTOM_MAX_FRAMES);uint16_t fd=(uint16_t)constrain(dly.toInt(),20,2000);uint8_t cc=(uint8_t)constrain(ccs.toInt(),1,(int)CUSTOM_MAX_COLORS);name.trim();if(name.length()==0)name="My Animation";if(name.length()>18)name=name.substring(0,18);
  CRGB parsedColors[CUSTOM_MAX_COLORS];int cp=0;uint8_t parsedCount=0;while(cp<=(int)colors.length()&&parsedCount<cc){int comma=colors.indexOf(',',cp);String token=comma<0?colors.substring(cp):colors.substring(cp,comma);CRGB col;if(!parseHex6Token(token,col)){server.send(400,"text/plain","Invalid custom palette color.");return;}parsedColors[parsedCount++]=col;if(comma<0)break;cp=comma+1;}if(parsedCount!=cc){server.send(400,"text/plain","Custom palette color count mismatch.");return;}
  uint8_t parsedFrames[CUSTOM_MAX_FRAMES][64];memset(parsedFrames,0,sizeof(parsedFrames));for(uint8_t f=0;f<fc;f++){if(pos>=(int)body.length()){server.send(400,"text/plain","Missing custom frame data.");return;}String frame=customTakeField(body,pos);if(frame.length()!=64){server.send(400,"text/plain","Each custom frame must contain 64 cells.");return;}for(uint8_t i=0;i<64;i++){char ch=frame.charAt(i);if(ch<'0'||ch>'8'){server.send(400,"text/plain","Custom frame cells must be 0..8.");return;}uint8_t idx=(uint8_t)(ch-'0');if(idx>cc){server.send(400,"text/plain","A frame uses a color outside the palette.");return;}parsedFrames[f][i]=idx;}}
  customAnimationColorCount=cc;customAnimationFrameCount=fc;customAnimationFrameDelay=fd;customAnimationRepeatSeconds=repeat;customAnimationFrame=0;customActiveSlot=slot;for(uint8_t i=0;i<CUSTOM_MAX_COLORS;i++)customAnimationColors[i]=parsedColors[i];memcpy(customAnimationFrames,parsedFrames,sizeof(parsedFrames));saveAnimationPalettes();saveCustomSlotToNVS(slot,name.c_str());saveAllSettings();server.send(200,"text/plain","Custom animation '"+name+"' saved in Slot "+String(slot+1)+" and hard-saved to ESP32 flash.");
}

void handleCustomSlotDelete() {if(!server.hasArg("slot")){server.send(400,"text/plain","Missing slot.");return;}uint8_t slot=(uint8_t)constrain(server.arg("slot").toInt(),0,(int)CUSTOM_SLOT_COUNT-1);deleteCustomSlotNVS(slot);if(slot==customActiveSlot){setDefaultCustomAnimation();customActiveSlot=0;saveAnimationPalettes();}server.send(200,"text/plain","Custom slot deleted.");}

// ============================================================
// HTTP: Tracking and sensitivity
// ============================================================

void handleTracking() {
  pieceTrackingEnabled = !pieceTrackingEnabled;
  saveSensitivitySettings();

  // Do not stop an animation when tracking is toggled.
  // Tracking is shown only when animations/games are not owning the LEDs.
  if (!gameActive && animationMode == ANIM_NONE) {
    if (pieceTrackingEnabled) {
      updatePieceTrackingLEDs();
    } else {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
    }
  }

  server.send(200, "text/plain", pieceTrackingEnabled ? "TRACKING ON" : "TRACKING OFF");
}

void handleSensitivity() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing value");
    return;
  }

  responseSensitivity = (uint8_t)constrain(server.arg("value").toInt(), 1, 10);
  memset(stableCounter, 0, sizeof(stableCounter));
  saveSensitivitySettings();
  server.send(200, "text/plain", String(responseSensitivity));
}

void handleSettle() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing value");
    return;
  }

  scanSettleUs = (uint16_t)constrain(server.arg("value").toInt(), 2, 100);
  saveSensitivitySettings();
  server.send(200, "text/plain", String(scanSettleUs));
}

// ============================================================
// HTTP: GPIO wiring
// ============================================================
void handlePinsGet() {
  String json = "{\"rowPins\":[";
  for(uint8_t i=0;i<8;i++){json += String(defaultRowPins[i]); if(i<7)json += ",";}
  json += "],\"colPins\":[";
  for(uint8_t i=0;i<8;i++){json += String(defaultColPins[i]); if(i<7)json += ",";}
  json += "]}";
  server.send(200,"application/json",json);
}

bool parsePinCsv(const String &src, uint8_t out[8], bool outputPins) {
  int pos=0;
  for(uint8_t i=0;i<8;i++){
    int comma=src.indexOf(',',pos);
    String t=(comma<0)?src.substring(pos):src.substring(pos,comma);
    t.trim();
    if(t.length()==0)return false;
    int v=t.toInt();
    if(v<0||v>39||!pinAllowedForMatrix((uint8_t)v,outputPins))return false;
    out[i]=(uint8_t)v;
    if(comma<0){if(i!=7)return false;pos=src.length();}else pos=comma+1;
  }
  return pos >= (int)src.length();
}

void handlePinsSet() {
  if(!server.hasArg("rows")||!server.hasArg("cols")){server.send(400,"text/plain","Missing rows/cols.");return;}
  uint8_t newRows[8],newCols[8];
  if(!parsePinCsv(server.arg("rows"),newRows,true)||!parsePinCsv(server.arg("cols"),newCols,false)){server.send(400,"text/plain","Invalid GPIO list. Check supported ESP32 GPIOs, uniqueness and LED GPIO 2.");return;}
  if(!pinsUniqueAll(newRows,newCols)){server.send(400,"text/plain","A GPIO cannot be used by more than one matrix line.");return;}
  for(uint8_t r=0;r<8;r++){pinMode(defaultRowPins[r],INPUT);}
  for(uint8_t c=0;c<8;c++){pinMode(defaultColPins[c],INPUT);}
  memcpy(defaultRowPins,newRows,sizeof(defaultRowPins));
  memcpy(defaultColPins,newCols,sizeof(defaultColPins));
  setupMatrixPins();
  memset(candidateMatrix,0,sizeof(candidateMatrix));memset(stableCounter,0,sizeof(stableCounter));memset(matrixState,0,sizeof(matrixState));memset(previousMatrixState,0,sizeof(previousMatrixState));
  savePinConfig();
  server.send(200,"text/plain","GPIO wiring updated and saved. Physically wire the matrix to these pins.");
}

// ============================================================
// HTTP: Arcade games
// ============================================================

void handleGameStart() {
  String type = server.hasArg("type") ? server.arg("type") : "two";
  gameStartSelected(type);
  server.send(200, "text/plain", gameStatus);
}

void handleGameStop() {
  gameStop();
  server.send(200, "text/plain", gameStatus);
}

// ============================================================
// HTTP: Mapping
// ============================================================

void sendMappingJson() {
  String json = "{";

  json += "\"rowPins\":[";
  for (uint8_t i = 0; i < 8; i++) {
    json += String(defaultRowPins[i]);
    if (i < 7) json += ",";
  }
  json += "],";

  json += "\"colPins\":[";
  for (uint8_t i = 0; i < 8; i++) {
    json += String(defaultColPins[i]);
    if (i < 7) json += ",";
  }
  json += "],";

  json += "\"rowMap\":[";
  for (uint8_t i = 0; i < 8; i++) {
    json += String(rowMap[i]);
    if (i < 7) json += ",";
  }
  json += "],";

  json += "\"colMap\":[";
  for (uint8_t i = 0; i < 8; i++) {
    json += String(colMap[i]);
    if (i < 7) json += ",";
  }
  json += "],";

  json += "\"rowText\":\"";
  json += jsonEscape(mappingToText(rowMap, defaultRowPins));
  json += "\",";

  json += "\"colText\":\"";
  json += jsonEscape(mappingToText(colMap, defaultColPins));
  json += "\"}";

  server.send(200, "application/json", json);
}

void handleMapping() {
  sendMappingJson();
}

bool parseMapString(const String &input, uint8_t output[8]) {
  int start = 0;

  for (uint8_t i = 0; i < 8; i++) {
    int comma = input.indexOf(',', start);
    String part;

    if (comma == -1) part = input.substring(start);
    else part = input.substring(start, comma);

    int value = part.toInt();
    if (value < 0 || value > 7) return false;

    output[i] = (uint8_t)value;

    if (comma == -1) {
      if (i != 7) return false;
      break;
    }

    start = comma + 1;
  }

  return mappingIsUnique(output);
}

void handleMappingSet() {
  if (!server.hasArg("rows") || !server.hasArg("cols")) {
    server.send(400, "text/plain", "Missing rows or cols");
    return;
  }

  uint8_t newRows[8];
  uint8_t newCols[8];

  if (!parseMapString(server.arg("rows"), newRows) ||
      !parseMapString(server.arg("cols"), newCols)) {
    server.send(400, "text/plain", "Invalid mapping. Use each physical index 0..7 exactly once.");
    return;
  }

  memcpy(rowMap, newRows, sizeof(rowMap));
  memcpy(colMap, newCols, sizeof(colMap));
  saveMapping();

  if (pieceTrackingEnabled) updatePieceTrackingLEDs();

  server.send(200, "text/plain", "Mapping saved");
}

void handleMappingReverse() {
  if (!server.hasArg("axis")) {
    server.send(400, "text/plain", "Missing axis");
    return;
  }

  String axis = server.arg("axis");
  axis.toLowerCase();

  if (axis == "row") {
    for (uint8_t i = 0; i < 4; i++) {
      uint8_t t = rowMap[i];
      rowMap[i] = rowMap[7 - i];
      rowMap[7 - i] = t;
    }
  } else if (axis == "col") {
    for (uint8_t i = 0; i < 4; i++) {
      uint8_t t = colMap[i];
      colMap[i] = colMap[7 - i];
      colMap[7 - i] = t;
    }
  } else {
    server.send(400, "text/plain", "Axis must be row or col");
    return;
  }

  saveMapping();
  if (pieceTrackingEnabled) updatePieceTrackingLEDs();
  server.send(200, "text/plain", "Mapping reversed and saved");
}

void handleMappingReset() {
  resetMapping();
  if (pieceTrackingEnabled) updatePieceTrackingLEDs();
  server.send(200, "text/plain", "Default mapping restored and saved");
}

// ============================================================
// HTTP: Calibration
// ============================================================

void handleCalibrationReset() {
  calibrationReset();
  server.send(200, "text/plain", calibrationStatus);
}

void handleCalibrationCapture() {
  if (!calibrationRunning) {
    server.send(400, "text/plain", "Calibration is not running. Press START 64-SQUARE CALIBRATION first.");
    return;
  }

  calibrationCapture();
  server.send(200, "text/plain", calibrationStatus);
}

void handleCalibrationResume() {
  calibrationResume();
  server.send(200, "text/plain", calibrationStatus);
}

void handleCalibrationPause() {
  calibrationRunning = false;
  saveCalibrationProgress(false);
  calibrationStatus =
    "Calibration paused. Saved progress is still in flash. Press RESUME SAVED to continue.";
  server.send(200, "text/plain", calibrationStatus);
}

void handleCalibrationClear() {
  calibrationRunning = false;
  calibrationStep = 0;
  memset(calibrationCaptured, 0, sizeof(calibrationCaptured));
  memset(calibrationPhysicalRow, 0, sizeof(calibrationPhysicalRow));
  memset(calibrationPhysicalCol, 0, sizeof(calibrationPhysicalCol));
  clearCalibrationProgressStorage();
  setDefaultMapping();
  saveMapping();
  calibrationStatus = "Calibration cleared. Mapping returned to default order.";
  server.send(200, "text/plain", calibrationStatus);
}

// ============================================================
// HTTP: Status JSON
// ============================================================

String animationModeName() {
  switch (animationMode) {
    case ANIM_RAINBOW: return "RAINBOW";
    case ANIM_WAVE: return "WAVE";
    case ANIM_CHASE: return "CHASE";
    case ANIM_SCAN: return "SCAN";
    case ANIM_BREATH: return "BREATH";
    case ANIM_RANDOM: return "RANDOM";
    case ANIM_CENTER_OUT: return "CENTER OUT";
    case ANIM_CUSTOM: return "CUSTOM";
    default: return "OFF";
  }
}

void handleStatus() {
  // The matrix is scanned continuously in loop(), so status only reports the latest data.
  String json = "{";

  json += "\"ssid\":\"";
  json += jsonEscape(String(AP_SSID));
  json += "\",";
  json += "\"wifiMode\":\"ACCESS POINT\",";
  json += "\"uptimeMs\":";
  json += String(millis());
  json += ",";
  json += "\"hardwareOnline\":true,";

  json += "\"ip\":\"";
  json += WiFi.softAPIP().toString();
  json += "\",";

  json += "\"active\":";
  json += String(activeSwitchCount());
  json += ",";

  json += "\"sensitivity\":";
  json += String(responseSensitivity);
  json += ",";

  json += "\"stableScans\":";
  json += String(requiredStableScans());
  json += ",";

  json += "\"settleUs\":";
  json += String(scanSettleUs);
  json += ",";

  json += "\"brightness\":";
  json += String(brightness);
  json += ",";

  json += "\"speed\":";
  json += String(animationSpeed);
  json += ",";

  json += "\"tracking\":";
  json += pieceTrackingEnabled ? "true" : "false";
  json += ",";

  char colorBuf[8];
  snprintf(colorBuf, sizeof(colorBuf), "#%02X%02X%02X", animationColor1.r, animationColor1.g, animationColor1.b);
  json += "\"animationColor1\":\"";
  json += colorBuf;
  json += "\",";

  snprintf(colorBuf, sizeof(colorBuf), "#%02X%02X%02X", animationColor2.r, animationColor2.g, animationColor2.b);
  json += "\"animationColor2\":\"";
  json += colorBuf;
  json += "\",";

  uint8_t currentPaletteCount = 0;
  if (animationMode >= ANIM_RAINBOW && animationMode <= ANIM_CENTER_OUT) currentPaletteCount = animationPaletteCounts[animationPaletteIndex(animationMode)];
  else if (animationMode == ANIM_CUSTOM) currentPaletteCount = customAnimationColorCount;
  json += "\"animationColorCount\":"; json += String(currentPaletteCount); json += ",";
  json += "\"animationColors\":[";
  if (animationMode >= ANIM_RAINBOW && animationMode <= ANIM_CENTER_OUT) {
    uint8_t pidx = animationPaletteIndex(animationMode);
    for (uint8_t i=0;i<animationPaletteCounts[pidx];i++) {
      snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",animationPalettes[pidx][i].r,animationPalettes[pidx][i].g,animationPalettes[pidx][i].b);
      json += "\"" + String(colorBuf) + "\""; if (i+1<animationPaletteCounts[pidx]) json += ",";
    }
  } else if (animationMode == ANIM_CUSTOM) {
    for (uint8_t i=0;i<customAnimationColorCount;i++) {
      snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",customAnimationColors[i].r,customAnimationColors[i].g,customAnimationColors[i].b);
      json += "\"" + String(colorBuf) + "\""; if (i+1<customAnimationColorCount) json += ",";
    }
  }
  json += "],";

  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",pieceTrackingColor.r,pieceTrackingColor.g,pieceTrackingColor.b);
  json += "\"trackingColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameP1Color.r,gameP1Color.g,gameP1Color.b);
  json += "\"gameP1Color\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameP2Color.r,gameP2Color.g,gameP2Color.b);
  json += "\"gameP2Color\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameGuideColor.r,gameGuideColor.g,gameGuideColor.b);
  json += "\"gameGuideColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameWarningColor.r,gameWarningColor.g,gameWarningColor.b);
  json += "\"gameWarningColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameLegalColor.r,gameLegalColor.g,gameLegalColor.b);
  json += "\"gameLegalColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameLiftColor.r,gameLiftColor.g,gameLiftColor.b);
  json += "\"gameLiftColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameTargetColor.r,gameTargetColor.g,gameTargetColor.b);
  json += "\"gameTargetColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameEnemyColor.r,gameEnemyColor.g,gameEnemyColor.b);
  json += "\"gameEnemyColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameBallColor.r,gameBallColor.g,gameBallColor.b);
  json += "\"gameBallColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameWallColor.r,gameWallColor.g,gameWallColor.b);
  json += "\"gameWallColor\":\""; json += colorBuf; json += "\",";
  snprintf(colorBuf,sizeof(colorBuf),"#%02X%02X%02X",gameTrailColor.r,gameTrailColor.g,gameTrailColor.b);
  json += "\"gameTrailColor\":\""; json += colorBuf; json += "\",";
  json += "\"customFrameCount\":"; json += String(customAnimationFrameCount); json += ",";
  json += "\"customFrameDelay\":"; json += String(customAnimationFrameDelay); json += ",";
  json += "\"customRepeatSeconds\":"; json += String(customAnimationRepeatSeconds); json += ",";
  json += "\"customActiveSlot\":"; json += String(customActiveSlot); json += ",";
  json += "\"customActiveName\":\""; json += jsonEscape(customSlotName(customActiveSlot)); json += "\",";

  json += "\"animation\":\"";
  json += animationModeName();
  json += "\",";

  json += "\"gameActive\":";
  json += gameActive ? "true" : "false";
  json += ",";

  json += "\"gameScore\":";
  json += String(gameScore);
  json += ",";

  json += "\"gameMoves\":";
  json += String(gameMoves);
  json += ",";

  json += "\"gameTarget\":\"";
  if (gameActive) json += squareName(gameTargetRow, gameTargetCol);
  else json += "--";
  json += "\",";

  json += "\"gameStatus\":\"";
  json += jsonEscape(gameStatus);
  json += "\",";

  json += "\"gameMode\":\"";
  json += jsonEscape(gameModeName());
  json += "\",";

  json += "\"gameLives\":"; json += String(gameLives); json += ",";
  json += "\"pongP1Score\":"; json += String(pongScore1); json += ",";
  json += "\"pongP2Score\":"; json += String(pongScore2); json += ",";
  json += "\"tetrisLines\":"; json += String(tetrisLines); json += ",";
  json += "\"snakeLength\":"; json += String(snakeLength); json += ",";
  uint16_t whackRemaining = 0;
  if (gameMode == GAME_WHACK && gameActive) {
    unsigned long elapsed = millis() - whackStartTime;
    whackRemaining = (elapsed >= WHACK_GAME_MS) ? 0 : (uint16_t)((WHACK_GAME_MS - elapsed) / 1000);
  }
  json += "\"whackSeconds\":"; json += String(whackRemaining); json += ",";

  json += "\"gameP1R\":"; json += String(gameP1Row); json += ",";
  json += "\"gameP1C\":"; json += String(gameP1Col); json += ",";
  json += "\"gameP2R\":"; json += String(gameP2Row); json += ",";
  json += "\"gameP2C\":"; json += String(gameP2Col); json += ",";
  json += "\"gameTargetR\":"; json += String(gameTargetRow); json += ",";
  json += "\"gameTargetC\":"; json += String(gameTargetCol); json += ",";
  json += "\"gameWaiting\":"; json += gameWaiting ? "true" : "false"; json += ",";
  json += "\"gameMovingPlayer\":"; json += String(gameMovingPlayer); json += ",";
  json += "\"gameGrid\":[";
  for (uint8_t i = 0; i < 64; i++) { json += String(gameGrid[i]); if (i < 63) json += ","; }
  json += "],";

  json += "\"gameDetails\":{";
  json += "\"breakoutBallR\":"; json += String(breakoutBallR); json += ",";
  json += "\"breakoutBallC\":"; json += String(breakoutBallC); json += ",";
  json += "\"breakoutPaddleC\":"; json += String(breakoutPaddleC); json += ",";
  json += "\"breakoutBricksLeft\":"; json += String(breakoutBricksLeft); json += ",";
  json += "\"breakoutLives\":"; json += String(breakoutLives); json += ",";
  json += "\"spaceInvadersWave\":"; json += String(siWave); json += ",";
  json += "\"spaceInvadersShipC\":"; json += String(siShipC); json += ",";
  json += "\"spaceInvadersMissileR\":"; json += String(siMissileR); json += ",";
  json += "\"spaceInvadersMissileC\":"; json += String(siMissileC); json += ",";
  json += "\"frogR\":"; json += String(frogR); json += ",";
  json += "\"frogC\":"; json += String(frogC); json += ",";
  json += "\"flappyBirdR\":"; json += String(flappyBirdR); json += ",";
  json += "\"flappyBirdC\":"; json += String(flappyBirdC); json += ",";
  json += "\"flappyPipeX\":"; json += String(flappyPipeX); json += ",";
  json += "\"flappyGapStart\":"; json += String(flappyGapStart); json += ",";
  json += "\"pongP1R\":"; json += String(pongP1R); json += ",";
  json += "\"pongP2R\":"; json += String(pongP2R); json += ",";
  json += "\"pongBallR\":"; json += String(pongBallR); json += ",";
  json += "\"pongBallC\":"; json += String(pongBallC); json += ",";
  json += "\"tronP1R\":"; json += String(tronP1R); json += ",";
  json += "\"tronP1C\":"; json += String(tronP1C); json += ",";
  json += "\"tronP2R\":"; json += String(tronP2R); json += ",";
  json += "\"tronP2C\":"; json += String(tronP2C); json += ",";
  json += "\"tankP1R\":"; json += String(tankP1R); json += ",";
  json += "\"tankP1C\":"; json += String(tankP1C); json += ",";
  json += "\"tankP2R\":"; json += String(tankP2R); json += ",";
  json += "\"tankP2C\":"; json += String(tankP2C); json += ",";
  json += "\"tankBulletR\":"; json += String(tankBulletR); json += ",";
  json += "\"tankBulletC\":"; json += String(tankBulletC); json += ",";
  json += "\"tetrisShape\":"; json += String(tetrisShape); json += ",";
  json += "\"tetrisRotation\":"; json += String(tetrisRotation); json += ",";
  json += "\"tetrisX\":"; json += String(tetrisX); json += ",";
  json += "\"tetrisY\":"; json += String(tetrisY); json += ",";
  json += "\"tetrisLines\":"; json += String(tetrisLines); json += ",";
  json += "\"snakeHeadIndex\":"; json += String(snakeHeadIndex); json += ",";
  json += "\"snakeFoodR\":"; json += String(snakeFoodR); json += ",";
  json += "\"snakeFoodC\":"; json += String(snakeFoodC); json += ",";
  json += "\"mineCursorR\":"; json += String(mineCursorR); json += ",";
  json += "\"mineCursorC\":"; json += String(mineCursorC); json += ",";
  json += "\"mineRevealedSafe\":"; json += String(mineRevealedSafe); json += ",";
  json += "\"mineCountTotal\":"; json += String(mineCountTotal); json += ",";
  json += "\"whackPlayerR\":"; json += String(whackPlayerR); json += ",";
  json += "\"whackPlayerC\":"; json += String(whackPlayerC); json += ",";
  json += "\"whackMoleR\":"; json += String(whackMoleR); json += ",";
  json += "\"whackMoleC\":"; json += String(whackMoleC); json += ",";
  json += "\"whackHits\":"; json += String(whackHits);
  json += "},";

  json += "\"lastSquare\":\"";
  json += jsonEscape(lastSquare);
  json += "\",";

  json += "\"lastEvent\":\"";
  json += jsonEscape(lastEvent);
  json += "\",";

  json += "\"rawRowMask\":";
  json += String(rawRowMask);
  json += ",";

  json += "\"rawColMask\":";
  json += String(rawColMask);
  json += ",";

  json += "\"rawActiveCells\":";
  json += String(rawActiveCells);
  json += ",";

  json += "\"multipleRawDetected\":";
  json += multipleRawDetected ? "true" : "false";
  json += ",";

  json += "\"diagnosticMessage\":\"";
  json += jsonEscape(diagnosticMessage);
  json += "\",";

  json += "\"calibrationRunning\":";
  json += calibrationRunning ? "true" : "false";
  json += ",";

  json += "\"calibrationSummary\":\"";
  json += jsonEscape(calibrationSummary);
  json += "\",";

  json += "\"calibrationStepNumber\":";
  json += String(calibrationStep + 1);
  json += ",";

  json += "\"calibrationStepName\":\"";
  if (calibrationStep < 64) json += squareName(calibrationStep / 8, calibrationStep % 8);
  else json += "DONE";
  json += "\",";

  json += "\"calibrationStatus\":\"";
  json += jsonEscape(calibrationStatus);
  json += "\",";

  uint8_t calibratedCount = 0;
  for (uint8_t i = 0; i < 64; i++) {
    if (calibrationCaptured[i]) calibratedCount++;
  }
  json += "\"calibratedSquares\":";
  json += String(calibratedCount);
  json += ",";

  json += "\"chip\":\"ESP32-WROOM-32U\",";
  json += "\"heap\":";
  json += String(ESP.getFreeHeap());
  json += ",";

  json += "\"leds\":";
  json += String(NUM_LEDS);
  json += ",";

  json += "\"rowPins\":[";
  for (uint8_t i = 0; i < 8; i++) {
    json += String(defaultRowPins[i]);
    if (i < 7) json += ",";
  }
  json += "],";

  json += "\"colPins\":[";
  for (uint8_t i = 0; i < 8; i++) {
    json += String(defaultColPins[i]);
    if (i < 7) json += ",";
  }
  json += "],";

  json += "\"rowMap\":[";
  for (uint8_t i = 0; i < 8; i++) {
    json += String(rowMap[i]);
    if (i < 7) json += ",";
  }
  json += "],";

  json += "\"colMap\":[";
  for (uint8_t i = 0; i < 8; i++) {
    json += String(colMap[i]);
    if (i < 7) json += ",";
  }
  json += "],";

  json += "\"rowText\":\"";
  json += jsonEscape(mappingToText(rowMap, defaultRowPins));
  json += "\",";

  json += "\"colText\":\"";
  json += jsonEscape(mappingToText(colMap, defaultColPins));
  json += "\",";

  json += "\"ledColors\":[";
  char ledColorBuf[8];
  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      uint8_t li = chessToLED(r, c);
      snprintf(ledColorBuf, sizeof(ledColorBuf), "#%02X%02X%02X", leds[li].r, leds[li].g, leds[li].b);
      json += "\""; json += ledColorBuf; json += "\"";
      if (r != ROWS - 1 || c != COLS - 1) json += ",";
    }
  }
  json += "],";

  json += "\"matrix\":[";
  for (uint8_t r = 0; r < ROWS; r++) {
    json += "[";
    for (uint8_t c = 0; c < COLS; c++) {
      json += matrixState[r][c] ? "true" : "false";
      if (c < COLS - 1) json += ",";
    }
    json += "]";
    if (r < ROWS - 1) json += ",";
  }
  json += "]";

  json += "}";

  addSmartChessCors();
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0", true);
  server.sendHeader("Pragma", "no-cache", true);
  server.send(200, "application/json", json);
}

void handleNotFound() {
  server.send(404, "text/plain", "404 - Not found");
}


// ============================================================
// SMART CHESS APP BRIDGE
// ============================================================
// The app is a separate UI layer. It reads /api/board and sends a compact
// 8x8 lighting map to /api/ledmap. The hardware layer never owns chess rules.
//
// LED map codes used by the app:
// 0 OFF
// 1 CYAN/WHITE piece
// 2 GRAY/BLACK piece
// 3 LEGAL destination
// 4 SELECTED piece
// 5 CAPTURE destination
// 6 LAST MOVE
// 7 HINT
// 8 CHECK / MISMATCH warning
void addSmartChessCors() {
  server.sendHeader("Access-Control-Allow-Origin", "*", true);
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS", true);
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type", true);
}

void handleSmartChessApp() {
  addSmartChessCors();
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0", true);
  server.sendHeader("Pragma", "no-cache", true);
  server.send_P(200, "text/html; charset=utf-8", SMART_CHESS_APP_HTML);
}

CRGB smartChessAppMapColor(uint8_t code) {
  switch (code) {
    case 1: return pieceTrackingColor;
    case 2: return pieceTrackingColor;
    case 3: return gameLegalColor;
    case 4: return gameLiftColor;
    case 5: return gameLegalColor;
    case 6: return gameLegalColor;
    case 7: return gameLiftColor;
    case 8: return gameWarningColor;
    default: return CRGB::Black;
  }
}

void handleSmartChessLEDMap() {
  addSmartChessCors();
  String map = server.arg("m");
  if (map.length() != 64) {
    server.send(400, "text/plain", "LED map must contain exactly 64 codes");
    return;
  }

  // App control temporarily takes ownership of the LEDs.
  animationMode = ANIM_NONE;
  gameActive = false;

  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t code = (uint8_t)(map[r * 8 + c] - '0');
      leds[chessToLED(r, c)] = smartChessAppMapColor(code);
    }
  }
  FastLED.show();
  server.send(200, "text/plain", "OK");
}

void handleSmartChessBoard() {
  addSmartChessCors();
  handleStatus();
}

// ============================================================
// Setup
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("===========================================");
  Serial.println("SMART CHESS  -  8x8 SENSITIVE TRACKING FIRMWARE");
  Serial.println("ESP32-WROOM-32U");
  Serial.println("===========================================");

  // LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  randomSeed(micros());
  gameStatus = "No game selected. Choose a game and press START SELECTED GAME.";

  // Load saved settings BEFORE startup animation so the saved colors,
  // brightness, sensitivity, tracking and speed are used immediately.
  loadPinConfig();
  loadMapping();
  loadAllSettings();
  loadGameColors();
  loadCalibrationProgress();
  if (!calibrationRunning) {
    bool anyCalibration = false;
    for (uint8_t i = 0; i < 64; i++) {
      if (calibrationCaptured[i]) { anyCalibration = true; break; }
    }
    if (anyCalibration && deriveFinalMapping()) {
      calibrationSummary = buildCalibrationSummary();
    }
  }
  FastLED.setBrightness(brightness);

  // Startup animation: center -> outward, using the saved animation colors.
  runStartupAnimation();

  // Reed matrix GPIO setup.
  setupMatrixPins();

  memset(matrixState, 0, sizeof(matrixState));
  memset(previousMatrixState, 0, sizeof(previousMatrixState));
  memset(candidateMatrix, 0, sizeof(candidateMatrix));
  memset(stableCounter, 0, sizeof(stableCounter));
  gameActive = false;
  gameHasBaseline = false;
  gameScore = 0;
  gameMoves = 0;

  // Start with tracking LEDs cleared.
  if (pieceTrackingEnabled) {
    updatePieceTrackingLEDs();
  }

  // Wi-Fi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println("Wi-Fi AP started");
  Serial.print("SSID: ");
  Serial.println(AP_SSID);
  Serial.print("Password: ");
  Serial.println(AP_PASSWORD);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  Serial.println("Row mapping:");
  Serial.println(mappingToText(rowMap, defaultRowPins));
  Serial.println("Column mapping:");
  Serial.println(mappingToText(colMap, defaultColPins));
  Serial.print("Detection sensitivity: ");
  Serial.println(responseSensitivity);
  Serial.print("Required stable scans: ");
  Serial.println(requiredStableScans());
  Serial.print("Scan settle time (us): ");
  Serial.println(scanSettleUs);
  Serial.print("Piece tracking: ");
  Serial.println(pieceTrackingEnabled ? "ON" : "OFF");

  // Routes.
  server.on("/", HTTP_GET, handleRoot);
  server.on("/app", HTTP_GET, handleSmartChessApp);
  server.on("/hardware", HTTP_GET, handleHardware);
  server.on("/pins", HTTP_GET, handlePinsGet);
  server.on("/pins/set", HTTP_GET, handlePinsSet);
  server.on("/app/", HTTP_GET, handleSmartChessApp);
  server.on("/api/board", HTTP_GET, handleSmartChessBoard);
  server.on("/api/ledmap", HTTP_GET, handleSmartChessLEDMap);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/led", HTTP_GET, handleLED);
  server.on("/alloff", HTTP_GET, handleAllOff);
  server.on("/allon", HTTP_GET, handleAllOn);
  server.on("/matrix", HTTP_GET, handleMatrix);
  server.on("/brightness", HTTP_GET, handleBrightness);
  server.on("/speed", HTTP_GET, handleSpeed);
  server.on("/animation", HTTP_GET, handleAnimation);
  server.on("/animation/colors", HTTP_GET, handleAnimationColors);
  server.on("/piece/colors", HTTP_GET, handlePieceColors);
  server.on("/custom", HTTP_GET, handleCustomGet);
  server.on("/custom/save", HTTP_POST, handleCustomSave);
  server.on("/custom/slots", HTTP_GET, handleCustomSlots);
  server.on("/custom/slot/load", HTTP_GET, handleCustomSlotLoad);
  server.on("/custom/slot/save", HTTP_POST, handleCustomSlotSave);
  server.on("/custom/slot/delete", HTTP_GET, handleCustomSlotDelete);
  server.on("/settings/save", HTTP_GET, handleHardSave);

  server.on("/game/start", HTTP_GET, handleGameStart);
  server.on("/game/stop", HTTP_GET, handleGameStop);

  server.on("/tracking", HTTP_GET, handleTracking);
  server.on("/sensitivity", HTTP_GET, handleSensitivity);
  server.on("/settle", HTTP_GET, handleSettle);

  server.on("/mapping", HTTP_GET, handleMapping);
  server.on("/mapping/set", HTTP_GET, handleMappingSet);
  server.on("/mapping/reverse", HTTP_GET, handleMappingReverse);
  server.on("/mapping/reset", HTTP_GET, handleMappingReset);

  server.on("/calibration/reset", HTTP_GET, handleCalibrationReset);
  server.on("/calibration/resume", HTTP_GET, handleCalibrationResume);
  server.on("/calibration/pause", HTTP_GET, handleCalibrationPause);
  server.on("/calibration/clear", HTTP_GET, handleCalibrationClear);
  server.on("/calibration/capture", HTTP_GET, handleCalibrationCapture);

  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("Web server started");
  Serial.println("Open http://192.168.4.1 in your browser");
  Serial.println("Smart Chess app: http://192.168.4.1/app");
  Serial.println("===========================================");
}

// ============================================================
// Loop
// ============================================================

void loop() {
  server.handleClient();

  // Continuously scan the board instead of waiting for the browser.
  // This makes switch detection and LED movement much more responsive.
  unsigned long now = millis();
  if (now - lastMatrixScan >= MATRIX_SCAN_INTERVAL_MS) {
    lastMatrixScan = now;
    scanPhysicalMatrix(true);
  }

  gameUpdate();
  updateAnimation();
  delay(0);
}