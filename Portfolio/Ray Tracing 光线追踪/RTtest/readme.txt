---
这是一个文件备注。
时间: 2020年4月
---

[adjust_in_maya. 在三维软件中确定摄像机的位置]

01. 第一张渐变图（未使用vec3类）
02. 第一张渐变图（使用vec3类）
03. 蓝白天空渐变
04. 第一次添加球体
05. 可视化法线
06. 创建各种类文件后添加地面，big为图片放大后局部细节
07. 创建相机类，添加抗锯齿，big为图片放大后局部细节
08. 添加递归反射，渲染初始兰伯特球体
09. 伽马校正后的漫反射球体，已经表现出较好的阴影效果
10. 忽略近0值后淡化黑色噪点
11. 朗伯分布漫反射（理想散射）
12. 半球（法线）反射
13. 创建材质类，添加金属球
14. 金属材质添加粗糙度，银球1.0，金球0.3
15. 银球修改为透明材质，只折射不反射，空气折射率1.0，玻璃1.5
16. 紫球也修改为透明，出现黑点
17. 不符合斯涅尔定律的情况改为反射，黑点被修正
18. 添加Schlick近似修正反射，边缘修正明显
19. 空心玻璃球效果
20. 调整相机，广角视野
21. 修改相机为任意视角相机，远眺视野
22. 视角调小为30度，实现拉近效果
23. 具有景深的灵活相机，实现散焦模糊
24. 最终场景，随机放一些小球
25. 图片放大为500×300，采样增加为500

---
编译器：
- Microsoft Visual Studio Community 2017
- 版本 15.9.12
编程语言：
- C++
Maya版本：
- Autodesk Maya 2019 学生版
---