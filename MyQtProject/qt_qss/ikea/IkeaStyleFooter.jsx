import './IkeaStyleFooter.css';

const IkeaStyleFooter = ({ onNavigate }) => {
  const currentYear = new Date().getFullYear();

  const handleNavigation = (page) => {
    if (onNavigate) {
      onNavigate(page);
    }
  };

  return (
    <footer className="ikea-footer">
      {/* 主要内容区域 */}
      <div className="ikea-footer-main">
        <div className="ikea-footer-container">
          <div className="ikea-footer-grid">
            {/* 博客内容 */}
            <div className="ikea-footer-column">
              <h3 className="ikea-footer-heading">博客内容</h3>
              <ul className="ikea-footer-list">
                <li><a href="#" className="ikea-footer-link" onClick={(e) => { e.preventDefault(); handleNavigation('home'); }}>技术文章</a></li>
                <li><a href="#" className="ikea-footer-link" onClick={(e) => { e.preventDefault(); handleNavigation('home'); }}>项目展示</a></li>
                <li><a href="#" className="ikea-footer-link" onClick={(e) => { e.preventDefault(); handleNavigation('home'); }}>开发经验</a></li>
                <li><a href="#" className="ikea-footer-link" onClick={(e) => { e.preventDefault(); handleNavigation('home'); }}>学习笔记</a></li>
              </ul>
            </div>

            {/* 技术栈 */}
            <div className="ikea-footer-column">
              <h3 className="ikea-footer-heading">技术栈</h3>
              <ul className="ikea-footer-list">
                <li><a href="#" className="ikea-footer-link">React</a></li>
                <li><a href="#" className="ikea-footer-link">Node.js</a></li>
                <li><a href="#" className="ikea-footer-link">Express</a></li>
                <li><a href="#" className="ikea-footer-link">MySQL</a></li>
              </ul>
            </div>

            {/* 关于 */}
            <div className="ikea-footer-column">
              <h3 className="ikea-footer-heading">关于</h3>
              <ul className="ikea-footer-list">
                <li><a href="#about" className="ikea-footer-link" onClick={(e) => { e.preventDefault(); handleNavigation('about'); }}>关于我</a></li>
                <li><a href="#contact" className="ikea-footer-link" onClick={(e) => { e.preventDefault(); handleNavigation('contact'); }}>联系方式</a></li>
                <li><a href="#" className="ikea-footer-link">友情链接</a></li>
                <li><a href="#" className="ikea-footer-link">网站地图</a></li>
              </ul>
            </div>

            {/* 关注我们 */}
            <div className="ikea-footer-column">
              <h3 className="ikea-footer-heading">关注我们</h3>
              <div className="ikea-footer-social">
                <a href="https://github.com/Shigure-moon" target="_blank" rel="noopener noreferrer" className="ikea-footer-social-link">
                  <svg width="24" height="24" viewBox="0 0 24 24" fill="currentColor">
                    <path d="M12 0c-6.626 0-12 5.373-12 12 0 5.302 3.438 9.8 8.207 11.387.599.111.793-.261.793-.577v-2.234c-3.338.726-4.033-1.416-4.033-1.416-.546-1.387-1.333-1.756-1.333-1.756-1.089-.745.083-.729.083-.729 1.205.084 1.839 1.237 1.839 1.237 1.07 1.834 2.807 1.304 3.492.997.107-.775.418-1.305.762-1.604-2.665-.305-5.467-1.334-5.467-5.931 0-1.311.469-2.381 1.236-3.221-.124-.303-.535-1.524.117-3.176 0 0 1.008-.322 3.301 1.23.957-.266 1.983-.399 3.003-.404 1.02.005 2.047.138 3.006.404 2.291-1.552 3.297-1.23 3.297-1.23.653 1.653.242 2.874.118 3.176.77.84 1.235 1.911 1.235 3.221 0 4.609-2.807 5.624-5.479 5.921.43.372.823 1.102.823 2.222v3.293c0 .319.192.694.801.576 4.765-1.589 8.199-6.086 8.199-11.386 0-6.627-5.373-12-12-12z"/>
                  </svg>
                  GitHub
                </a>
                <a href="mailto:a993056494@163.com" className="ikea-footer-social-link">
                  <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                    <path d="M4 4h16c1.1 0 2 .9 2 2v12c0 1.1-.9 2-2 2H4c-1.1 0-2-.9-2-2V6c0-1.1.9-2 2-2z"></path>
                    <polyline points="22,6 12,13 2,6"></polyline>
                  </svg>
                  邮箱联系
                </a>
              </div>
              <div className="ikea-footer-newsletter">
                <p className="ikea-footer-newsletter-text">订阅更新通知</p>
                <div className="ikea-footer-newsletter-form">
                  <input type="email" placeholder="输入邮箱地址" className="ikea-footer-newsletter-input" />
                  <button className="ikea-footer-newsletter-btn">订阅</button>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      {/* 底部版权区域 */}
      <div className="ikea-footer-bottom">
        <div className="ikea-footer-container">
          <div className="ikea-footer-bottom-content">
            <div className="ikea-footer-logo-section">
              <img src="/img/logo_simple_white.png" alt="Shigure Logo" className="ikea-footer-logo" />
              <span className="ikea-footer-brand">Shigure的博客</span>
            </div>
            <div className="ikea-footer-legal">
              <a href="#privacy-policy" className="ikea-footer-legal-link" onClick={(e) => { e.preventDefault(); handleNavigation('privacy-policy'); }}>隐私政策</a>
              <a href="#terms-of-service" className="ikea-footer-legal-link" onClick={(e) => { e.preventDefault(); handleNavigation('terms-of-service'); }}>使用条款</a>
              <a href="#cookie-policy" className="ikea-footer-legal-link" onClick={(e) => { e.preventDefault(); handleNavigation('cookie-policy'); }}>Cookie政策</a>
            </div>
            <div className="ikea-footer-copyright">
              <p>© {currentYear} Shigure的博客. 保留所有权利.</p>
            </div>
          </div>
        </div>
      </div>
    </footer>
  );
};

export default IkeaStyleFooter;